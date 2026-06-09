#!/usr/bin/env python3
"""
Скрипт для удаления модуля ядра из initramfs (Debian/Ubuntu)
Строгий порядок операций с проверкой ошибок на каждом шаге
"""

import argparse
import subprocess
import shutil
import sys
import os
import re

def run_command(cmd, capture=False, description=""):
    """Выполняет команду, возвращает (success, output)"""
    try:
        result = subprocess.run(
            cmd,
            shell=True,
            capture_output=capture,
            text=True,
            check=True
        )
        return True, result.stdout if capture else ""
    except subprocess.CalledProcessError as e:
        if description:
            print(f"❌ ОШИБКА: {description}")
        if e.stderr:
            print(f"   {e.stderr.strip()}")
        return False, ""

def check_module_in_lib_modules(module_path, kernel_version):
    """Проверяет наличие модуля в /lib/modules"""
    dest_path = f"/lib/modules/{kernel_version}/{os.path.basename(module_path)}"
    
    if not os.path.exists(dest_path):
        print(f"   ⚠️  Модуль не найден в /lib/modules/{kernel_version}/")
        return False
    return True

def get_module_name(module_path):
    """Извлекает имя модуля из пути"""
    basename = os.path.basename(module_path)
    name = re.sub(r'\.ko(\..*)?$', '', basename)
    return name, name.replace('-', '_')

def remove_from_lib_modules(module_path, kernel_version):
    """Удаляет модуль из /lib/modules/(version)/"""
    print(f"🗑️  Шаг 1/5: Удаление модуля из /lib/modules/{kernel_version}/...")
    
    dest_path = f"/lib/modules/{kernel_version}/{os.path.basename(module_path)}"
    
    if not os.path.exists(dest_path):
        print(f"   ⚠️  Файл не найден: {dest_path}")
        print(f"   ✅ Пропущено (уже удалён)")
        return True
    
    try:
        os.remove(dest_path)
        print(f"   ✅ Модуль удалён: {dest_path}")
        return True
    except Exception as e:
        print(f"❌ ОШИБКА: Не удалось удалить модуль: {e}")
        return False

def remove_from_initramfs_config(module_name):
    """Удаляет модуль из /etc/initramfs-tools/modules"""
    print(f"📝 Шаг 2/5: Удаление из /etc/initramfs-tools/modules...")
    
    config_file = "/etc/initramfs-tools/modules"
    
    if not os.path.exists(config_file):
        print(f"   ⚠️  Файл конфигурации не найден: {config_file}")
        print(f"   ✅ Пропущено")
        return True
    
    try:
        with open(config_file, 'r') as f:
            lines = f.readlines()
        
        # Фильтруем строки с именем модуля
        new_lines = []
        removed = False
        for line in lines:
            stripped = line.strip()
            if stripped == module_name or stripped.startswith(module_name + ' '):
                removed = True
                print(f"   🗑️  Удалена строка: {stripped}")
            else:
                new_lines.append(line)
        
        if not removed:
            print(f"   ⚠️  Модуль '{module_name}' не найден в конфигурации")
            print(f"   ✅ Пропущено")
            return True
        
        # Записываем обновлённый файл
        with open(config_file, 'w') as f:
            f.writelines(new_lines)
        
        print(f"   ✅ Модуль '{module_name}' удалён из конфигурации")
        return True
    
    except Exception as e:
        print(f"❌ ОШИБКА: Не удалось обновить конфигурацию: {e}")
        return False

def run_depmod(kernel_version):
    """Выполняет depmod -a"""
    print(f"🔄 Шаг 3/5: Выполнение depmod -a {kernel_version}...")
    
    success, _ = run_command(
        f"depmod -a {kernel_version}",
        description="Не удалось выполнить depmod"
    )
    if not success:
        return False
    
    print("   ✅ depmod выполнен успешно")
    return True

def update_initramfs(kernel_version):
    """Пересобирает initramfs"""
    print(f"🔨 Шаг 4/5: Пересборка initramfs...")
    
    success, output = run_command(
        f"update-initramfs -u -k {kernel_version}",
        capture=True,
        description="Не удалось пересобрать initramfs"
    )
    if not success:
        return False
    
    print("   ✅ initramfs обновлён успешно")
    return True

def verify_not_in_initramfs(module_name, kernel_version):
    """Проверяет что модуль НЕ содержится в initramfs"""
    print(f"🔍 Шаг 5/5: Проверка отсутствия модуля в initramfs...")
    
    initramfs_path = f"/boot/initrd.img-{kernel_version}"
    
    if not os.path.exists(initramfs_path):
        print(f"❌ ОШИБКА: Файл initramfs не найден: {initramfs_path}")
        return False
    
    success, output = run_command(
        f"lsinitramfs {initramfs_path} | grep {module_name}",
        capture=True,
        description="Не удалось выполнить проверку lsinitramfs"
    )
    
    # Если grep нашёл что-то (output не пустой) - это ошибка
    if success and output and module_name in output:
        print(f"❌ ОШИБКА: Модуль ВСЁ ЕЩЁ содержится в initramfs!")
        print(f"   Найдено:")
        for line in output.strip().split('\n'):
            print(f"      {line}")
        print(f"\n🔧 Попробуйте выполнить вручную:")
        print(f"   update-initramfs -v -u -k {kernel_version}")
        return False
    
    print(f"   ✅ Модуль не найден в initramfs (удалён успешно)")
    return True

def main():
    parser = argparse.ArgumentParser(
        description='Удаление модуля ядра из initramfs (Debian/Ubuntu)'
    )
    parser.add_argument('module_path', help='Путь к .ko файлу модуля')
    parser.add_argument('kernel_version', help='Версия ядра (например, 6.8.0)')
    
    args = parser.parse_args()
    
    # Проверка прав root
    if os.geteuid() != 0:
        print("❌ ОШИБКА: Скрипт требует прав root")
        print("   Запустите: sudo python3 " + ' '.join(sys.argv))
        sys.exit(1)
    
    print("=" * 60)
    print("🗑️  УДАЛЕНИЕ МОДУЛЯ ИЗ INITRAMFS")
    print("=" * 60)
    print(f"   Модуль: {args.module_path}")
    print(f"   Ядро: {args.kernel_version}")
    print("=" * 60)
    print()
    
    # Проверка существования файла модуля (исходного)
    if not os.path.exists(args.module_path):
        print(f"⚠️  ПРЕДУПРЕЖДЕНИЕ: Исходный файл не найден: {args.module_path}")
        print(f"   Продолжим удаление из системы...")
    print()
    
    # Извлекаем имя модуля
    module_name, module_sys_name = get_module_name(args.module_path)
    print(f"   Имя модуля: {module_name}")
    print(f"   Имя в /sys/module: {module_sys_name}")
    print()
    
    # 1. Удаление из /lib/modules
    if not remove_from_lib_modules(args.module_path, args.kernel_version):
        print("\n" + "=" * 60)
        print("❌ ЗАВЕРШЕНО С ОШИБКОЙ")
        print("=" * 60)
        sys.exit(1)
    print()
    
    # 2. Удаление из конфигурации initramfs
    if not remove_from_initramfs_config(module_name):
        print("\n" + "=" * 60)
        print("❌ ЗАВЕРШЕНО С ОШИБКОЙ")
        print("=" * 60)
        sys.exit(1)
    print()
    
    # 3. depmod
    if not run_depmod(args.kernel_version):
        print("\n" + "=" * 60)
        print("❌ ЗАВЕРШЕНО С ОШИБКОЙ")
        print("=" * 60)
        sys.exit(1)
    print()
    
    # 4. update-initramfs
    if not update_initramfs(args.kernel_version):
        print("\n" + "=" * 60)
        print("❌ ЗАВЕРШЕНО С ОШИБКОЙ")
        print("=" * 60)
        sys.exit(1)
    print()
    
    # 5. Проверка отсутствия в initramfs
    if not verify_not_in_initramfs(module_name, args.kernel_version):
        print("\n" + "=" * 60)
        print("❌ ЗАВЕРШЕНО С ОШИБКОЙ: Модуль не удалён из initramfs")
        print("=" * 60)
        print("\n🔧 Возможные причины:")
        print("   1. Другой скрипт/хук добавляет модуль")
        print("   2. Модуль является зависимостью другого модуля")
        print("   3. Кэширование в initramfs-tools")
        print("\n📋 Для диагностики выполните:")
        print(f"   update-initramfs -v -u -k {args.kernel_version}")
        print(f"   lsinitramfs /boot/initrd.img-{args.kernel_version} | grep {module_name}")
        print(f"   cat /etc/initramfs-tools/modules")
        sys.exit(1)
    
    # Успешное завершение
    print("\n" + "=" * 60)
    print("✅ УСПЕШНО ЗАВЕРШЕНО")
    print("=" * 60)
    print(f"\n📌 Модуль '{module_name}' полностью удалён из:")
    print(f"   • /lib/modules/{args.kernel_version}/")
    print(f"   • /etc/initramfs-tools/modules")
    print(f"   • /boot/initrd.img-{args.kernel_version}")
    print(f"\n📌 Для полной очистки перезагрузите систему:")
    print(f"   sudo reboot")
    print("=" * 60)
    
    sys.exit(0)

if __name__ == '__main__':
    main()