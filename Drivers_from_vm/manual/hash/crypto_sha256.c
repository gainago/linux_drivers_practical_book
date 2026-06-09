/*
 * crypto_sha256.c
 */
#include <crypto/internal/hash.h>
#include <linux/module.h>

#define SHA256_LENGTH 32

static void show_hash_result(char *plain_text, char *hash_sha256)
{
	int i = 0;
	char str[SHA256_LENGTH * 2 + 1];
	pr_info("sha256 test for string: \"%s\"\n", plain_text);
	for (i = 0; i < SHA256_LENGTH; i++) {
		sprintf(&str[i * 2], "%02x",
			       	(unsigned char)hash_sha256[i]);
	}
	str[SHA256_LENGTH * 2] = 0;
	pr_info("%s\n", str);
}

static int crypto_sha256_init(void)
{
	char *plain_text = "This is a text";
	char hash_sha256[SHA256_LENGTH];
	struct crypto_shash *sha256;
	struct shash_desc *shash;

	sha256 = crypto_alloc_shash("sha256", 0, 0);
	if (IS_ERR(sha256))
		return -1;
	shash = kmalloc(sizeof(struct shash_desc) + 
			crypto_shash_descsize(sha256), GFP_KERNEL);
	if (!shash)
		return -ENOMEM;
	shash->tfm = sha256; /* Set transformation algorithm. */
	if (crypto_shash_init(shash))
		return -1;
	if (crypto_shash_update(shash, plain_text,
			       	strlen(plain_text)))
		return -1;
	if (crypto_shash_final(shash, hash_sha256))
		return -1;
	kfree(shash);
	crypto_free_shash(sha256);

	show_hash_result(plain_text, hash_sha256);

	return 0;
}

static void crypto_sha256_exit(void)
{
}

module_init(crypto_sha256_init);
module_exit(crypto_sha256_exit);

MODULE_DESCRIPTION("sha256 hash text");

MODULE_LICENSE("GPL");

