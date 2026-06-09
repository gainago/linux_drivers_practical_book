#!/usr/bin/env python3
"""
Скрипт для добавления модуля ядра в initramfs (Debian/Ubuntu)
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

def check_module_exists(module_path):
    """Проверяет существование файла модуля"""
    if not os.path.exists(module_path):
        print(f"❌ ОШИБКА: Модуль не найден: {module_path}")
        return False
    return True

def check_module_version(module_path, kernel_version):
    """Проверяет vermagic модуля"""
    print(f"🔍 Шаг 1/6: Проверка версии модуля...")
    
    success, output = run_command(
        f"modinfo -F vermagic {module_path}",
        capture=True,
        description="Не удалось получить vermagic модуля"
    )
    if not success:
        return False
    
    vermagic = output.strip()
    print(f"   Версия модуля: {vermagic}")
    print(f"   Требуется: {kernel_version}")
    
    if not vermagic.startswith(kernel_version):
        print(f"   ⚠️  ПРЕДУПРЕЖДЕНИЕ: версия модуля не совпадает!")
        response = input("   Продолжить? (y/n): ")
        if response.lower() != 'y':
            print("❌ ОШИБКА: Отменено пользователем")
            return False
    
    print("   ✅ Проверка версии пройдена")
    return True

def get_module_name(module_path):
    """Извлекает имя модуля из пути"""
    basename = os.path.basename(module_path)
    name = re.sub(r'\.ko(\..*)?$', '', basename)
    return name, name.replace('-', '_')

def copy_to_lib_modules(module_path, kernel_version):
    """Копирует модуль в /lib/modules/(version)/"""
    print(f"📋 Шаг 2/6: Копирование модуля в /lib/modules/{kernel_version}/...")
    
    dest_dir = f"/lib/modules/{kernel_version}"
    dest_path = f"{dest_dir}/{os.path.basename(module_path)}"
    
    if not os.path.exists(dest_dir):
        print(f"❌ ОШИБКА: Директория не существует: {dest_dir}")
        return False
    
    try:
        shutil.copy2(module_path, dest_path)
        print(f"   ✅ Модуль скопирован: {dest_path}")
        return True
    except Exception as e:
        print(f"❌ ОШИБКА: Не удалось скопировать модуль: {e}")
        return False

def add_to_initramfs_config(module_name):
    """Добавляет модуль в /etc/initramfs-tools/modules"""
    print(f"📝 Шаг 3/6: Добавление в /etc/initramfs-tools/modules...")
    
    config_file = "/etc/initramfs-tools/modules"
    
    # Проверяем существующее содержимое
    try:
        with open(config_file, 'r') as f:
            existing = f.read()
        
        if module_name in existing.split():
            print(f"   ⚠️  Модуль уже в конфигурации")
            return True
    except FileNotFoundError:
        print(f"   ⚠️  Файл конфигурации не найден, будет создан")
    except Exception as e:
        print(f"❌ ОШИБКА: Не удалось прочитать конфигурацию: {e}")
        return False
    
    # Добавляем модуль
    success, _ = run_command(
        f"echo '{module_name}' | tee -a {config_file}",
        description="Не удалось добавить модуль в конфигурацию"
    )
    if not success:
        return False
    
    print(f"   ✅ Модуль '{module_name}' добавлен в конфигурацию")
    return True

def run_depmod(kernel_version):
    """Выполняет depmod -a"""
    print(f"🔄 Шаг 4/6: Выполнение depmod -a {kernel_version}...")
    
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
    print(f"🔨 Шаг 5/6: Пересборка initramfs...")
    
    success, output = run_command(
        f"update-initramfs -u -k {kernel_version}",
        capture=True,
        description="Не удалось пересобрать initramfs"
    )
    if not success:
        return False
    
    print("   ✅ initramfs обновлён успешно")
    return True

def verify_in_initramfs(module_name, kernel_version):
    """Проверяет наличие модуля в initramfs"""
    print(f"🔍 Шаг 6/6: Проверка наличия модуля в initramfs...")
    
    initramfs_path = f"/boot/initrd.img-{kernel_version}"
    
    if not os.path.exists(initramfs_path):
        print(f"❌ ОШИБКА: Файл initramfs не найден: {initramfs_path}")
        return False
    
    success, output = run_command(
        f"lsinitramfs {initramfs_path} | grep {module_name}",
        capture=True,
        description="Не удалось выполнить проверку lsinitramfs"
    )
    
    if not success or not output or module_name not in output:
        print(f"❌ ОШИБКА: Модуль НЕ найден в initramfs!")
        print(f"   Ожидалось: {module_name}")
        print(f"   Проверьте: lsinitramfs {initramfs_path} | grep {module_name}")
        return False
    
    print(f"   ✅ Модуль найден в initramfs:")
    for line in output.strip().split('\n'):
        print(f"      {line}")
    return True

def main():
    parser = argparse.ArgumentParser(
        description='Добавление модуля ядра в initramfs (Debian/Ubuntu)'
    )
    parser.add_argument('module_path', help='Путь к .ko файлу модуля')
    parser.add_argument('kernel_version', help='Версия ядра (например, 6.8.0)')
    parser.add_argument('--skip-version-check', action='store_true',
                       help='Пропустить проверку версии модуля')
    
    args = parser.parse_args()
    
    # Проверка прав root
    if os.geteuid() != 0:
        print("❌ ОШИБКА: Скрипт требует прав root")
        print("   Запустите: sudo python3 " + ' '.join(sys.argv))
        sys.exit(1)
    
    print("=" * 60)
    print("🚀 ДОБАВЛЕНИЕ МОДУЛЯ В INITRAMFS")
    print("=" * 60)
    print(f"   Модуль: {args.module_path}")
    print(f"   Ядро: {args.kernel_version}")
    print("=" * 60)
    print()
    
    # Проверка существования модуля
    if not check_module_exists(args.module_path):
        sys.exit(1)
    
    # 1. Проверка версии (опционально)
    if not args.skip_version_check:
        if not check_module_version(args.module_path, args.kernel_version):
            print("\n❌ ЗАВЕРШЕНО С ОШИБКОЙ")
            sys.exit(1)
    else:
        print(f"⏭️  Шаг 1/6: Пропущено ( --skip-version-check )")
    print()
    
    # 2. Копирование в /lib/modules
    if not copy_to_lib_modules(args.module_path, args.kernel_version):
        print("\n❌ ЗАВЕРШЕНО С ОШИБКОЙ")
        sys.exit(1)
    print()
    
    # 3. Добавление в конфигурацию initramfs
    module_name, module_sys_name = get_module_name(args.module_path)
    if not add_to_initramfs_config(module_name):
        print("\n❌ ЗАВЕРШЕНО С ОШИБКОЙ")
        sys.exit(1)
    print()
    
    # 4. depmod
    if not run_depmod(args.kernel_version):
        print("\n❌ ЗАВЕРШЕНО С ОШИБКОЙ")
        sys.exit(1)
    print()
    
    # 5. update-initramfs
    if not update_initramfs(args.kernel_version):
        print("\n❌ ЗАВЕРШЕНО С ОШИБКОЙ")
        sys.exit(1)
    print()
    
    # 6. Проверка наличия в initramfs
    if not verify_in_initramfs(module_name, args.kernel_version):
        print("\n" + "=" * 60)
        print("❌ ЗАВЕРШЕНО С ОШИБКОЙ: Модуль не добавлен в initramfs")
        print("=" * 60)
        print("\n🔧 Возможные причины:")
        print("   1. Модуль имеет зависимости, которые не выполнены")
        print("   2. Ошибка в скриптах initramfs-tools")
        print("   3. Недостаточно прав доступа")
        print("\n📋 Для диагностики выполните:")
        print(f"   update-initramfs -v -u -k {args.kernel_version}")
        print(f"   lsinitramfs /boot/initrd.img-{args.kernel_version} | grep {module_name}")
        sys.exit(1)
    
    # Успешное завершение
    print("\n" + "=" * 60)
    print("✅ УСПЕШНО ЗАВЕРШЕНО")
    print("=" * 60)
    print(f"\n📌 Для загрузки модуля в initramfs:")
    print(f"   insmod /lib/modules/{args.kernel_version}/{os.path.basename(args.module_path)}")
    print(f"\n📌 Для отладки в GDB:")
    print(f"   1. Поставьте breakpoint: break hello_init")
    print(f"   2. Загрузите модуль: insmod /lib/modules/{args.kernel_version}/{os.path.basename(args.module_path)}")
    print(f"   3. Узнайте адрес: cat /sys/module/{module_sys_name}/sections/.text")
    print(f"   4. Подключите символы: add-symbol-file {args.module_path} 0x<адрес>")
    print("=" * 60)
    
    sys.exit(0)

if __name__ == '__main__':
    main()