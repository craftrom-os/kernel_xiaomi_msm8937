#include <linux/module.h>
#include <linux/init.h>
#include <linux/device.h>
#include <linux/fs.h>
#include <linux/slab.h>
#include <linux/err.h>
#include <linux/soc/qcom/smem.h>
#include <linux/platform_device.h>
#include <linux/qpnp/qpnp-adc.h>
#include <xiaomi-sdm439/mach.h>
#include "hqsys_pcba.h"

#define SMEM_ID_VENDOR1 135

static PCBA_CONFIG huaqin_pcba_config;

//static struct device_node  *huaqin_pcba_vadc_of_node;

//struct qpnp_vadc_chip *pcba_vadc;

extern void xiaomi_sdm439_mach_notify_hq_pcba_config(PCBA_CONFIG hq_pcba);

static void read_pcba_config_form_smem(void)
{
	size_t size;
	PCBA_CONFIG *pcba_config = NULL;
	pcba_config = (PCBA_CONFIG *)qcom_smem_get(QCOM_SMEM_HOST_ANY, SMEM_ID_VENDOR1, &size);
	if (IS_ERR(pcba_config) || !size) {
		printk(KERN_ERR "pcba config fail\n");

		huaqin_pcba_config = PCBA_UNKNOW;
	} else {
		printk(KERN_ERR "pcba config =%d.\n", *(pcba_config));
		if (*(pcba_config) > PCBA_UNKNOW && *(pcba_config) < PCBA_END) {
			huaqin_pcba_config = *pcba_config;
		} else {
			huaqin_pcba_config = PCBA_UNKNOW;
		}
	}

	xiaomi_sdm439_mach_notify_hq_pcba_config(huaqin_pcba_config);
}

PCBA_CONFIG xiaomi_sdm439_get_huaqin_pcba_config(void)
{
	return huaqin_pcba_config;
}
EXPORT_SYMBOL_GPL(xiaomi_sdm439_get_huaqin_pcba_config);


#if 0

int huaqin_pcba_vadc_probe(struct platform_device *ppcba_vadc)
{
	struct qpnp_vadc_result vadc_result;
	int64_t vadc;
	vadc = 0;
	printk("%s\n", __func__);
	huaqin_pcba_vadc_of_node = ppcba_vadc->dev.of_node;
	pcba_vadc = qpnp_get_vadc(&(ppcba_vadc->dev), "huaqin-pcba");
	if (!IS_ERR(pcba_vadc)) {
		qpnp_vadc_read(pcba_vadc, P_MUX1_1_1, &vadc_result);
		vadc = vadc_result.physical/1000;
		printk(KERN_ERR "%s %lld\n", __func__, vadc);
	}
	return 0;
}
static struct of_device_id huaqin_pcba_vadc_match_table[] = {
	{.compatible = "huaqin_pcba_vadc"},
	{}
};

static struct platform_driver huaqin_pcba_vadc_driver = {
	.probe = huaqin_pcba_vadc_probe,
	.driver = {
		.name = "huaqinc_pcba_vadc_8940",
		.of_match_table = huaqin_pcba_vadc_match_table,
		.owner = THIS_MODULE,
	},
};
static int __init huaqin_pcba_module_init(void)
{
	if (!xiaomi_sdm439_mach_get())
		return -ENODEV;

	platform_driver_register(&huaqin_pcba_vadc_driver);

	return 0;
}
#endif

static int __init huaqin_pcba_early_init(void)
{
	if (!xiaomi_sdm439_mach_get())
		return -ENODEV;

	read_pcba_config_form_smem();
	return 0;
}

subsys_initcall(huaqin_pcba_early_init); //before device_initcall

//late_initcall(huaqin_pcba_module_init);   //late initcall

MODULE_AUTHOR("ninjia <nijiayu@huaqin.com>");
MODULE_DESCRIPTION("huaqin sys pcba");
MODULE_LICENSE("GPL");
