/* ----------------------------------------------------------------------
*
* Copyright (c) 2017 ChipCraft Sp. z o.o. All rights reserved
*
* $Date: 2018-12-04 14:27:17 +0100 (Tue, 04 Dec 2018) $
* $Revision: 362 $
*
*  ----------------------------------------------------------------------
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions
* are met:
*   - Redistributions of source code must retain the above copyright
*     notice, this list of conditions and the following disclaimer.
*   - Redistributions in binary form must reproduce the above copyright
*     notice, this list of conditions and the following disclaimer in
*     the documentation and/or other materials provided with the
*     distribution.
*   - Neither the name of ChipCraft Sp. z o.o. nor the names of its
*     contributors may be used to endorse or promote products derived
*     from this software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
* "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
* LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
* FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
* COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
* INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
* BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
* LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
* CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
* LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
* ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
* POSSIBILITY OF SUCH DAMAGE.
 * -------------------------------------------------------------------- */

#include <ccproc.h>
#include <ccproc-utils.h>
#include <ccproc-amba.h>
#include <ccproc-amba-gpio.h>
#include <ccproc-amba-cfgregs.h>
#include <ccproc-amba-flash.h>
#include <flash.h>
#include <board.h>

/**
 * @brief Initialize the CCNV1 board
 */
void board_init(void)
{

    /* Enable GPIO controller */
    AMBA_GPIO_PTR->CTRL |= GPIO_CTRL_EN;

    /* Set UART0 to GPIO 0, 1, 2, 3 */
    AMBA_GPIO_PTR->ALTER_LO |= GPIO_CONFIG_MASK(0,GPIO_ALTER_0);
    AMBA_GPIO_PTR->ALTER_LO |= GPIO_CONFIG_MASK(1,GPIO_ALTER_0);
    AMBA_GPIO_PTR->ALTER_LO |= GPIO_CONFIG_MASK(2,GPIO_ALTER_0);
    AMBA_GPIO_PTR->ALTER_LO |= GPIO_CONFIG_MASK(3,GPIO_ALTER_0);

    /* Set UART1 to GPIO 4, 5, 6, 7 */
    AMBA_GPIO_PTR->ALTER_LO |= GPIO_CONFIG_MASK(4,GPIO_ALTER_0);
    AMBA_GPIO_PTR->ALTER_LO |= GPIO_CONFIG_MASK(5,GPIO_ALTER_0);
    AMBA_GPIO_PTR->ALTER_LO |= GPIO_CONFIG_MASK(6,GPIO_ALTER_0);
    AMBA_GPIO_PTR->ALTER_LO |= GPIO_CONFIG_MASK(7,GPIO_ALTER_0);

    /* Set I2C slave to GPIO 8, 9 */
    AMBA_GPIO_PTR->ALTER_LO |= GPIO_CONFIG_MASK(8,GPIO_ALTER_1);
    AMBA_GPIO_PTR->ALTER_LO |= GPIO_CONFIG_MASK(9,GPIO_ALTER_1);

    /* Set pull-up to GPIO 8, 9 */
    AMBA_GPIO_PTR->PULL_LO |= GPIO_CONFIG_MASK(8,GPIO_PULL_UP);
    AMBA_GPIO_PTR->PULL_LO |= GPIO_CONFIG_MASK(9,GPIO_PULL_UP);

    /* Set SPI slave to GPIO 12, 13, 14, 15 */
    AMBA_GPIO_PTR->ALTER_LO |= GPIO_CONFIG_MASK(12,GPIO_ALTER_1);
    AMBA_GPIO_PTR->ALTER_LO |= GPIO_CONFIG_MASK(13,GPIO_ALTER_1);
    AMBA_GPIO_PTR->ALTER_LO |= GPIO_CONFIG_MASK(14,GPIO_ALTER_1);
    AMBA_GPIO_PTR->ALTER_LO |= GPIO_CONFIG_MASK(15,GPIO_ALTER_1);

    /* Disable GPIO controller */
    AMBA_GPIO_PTR->CTRL &= ~GPIO_CTRL_EN;

    /* Enable APB1 Bridge */
    AMBA_APB0_CFG_PTR->APB1_CFG = AMBA_APB1_EN;

    /* Configure Flash Controller */
    flash_configure(CORE_FREQ,(uint8_t)FLASH_READ_WAIT_STATES_CALC(CORE_FREQ),1,0);

    /* Configure PLL */
    CFG_REGS_PTR->CFGREG_UNLOCK = CFGREG_UNLOCK_DEF;
    CFG_REGS_PTR->CFGREG_COREFREQ_PLL = CFGREG_COREFREQ_PLL_EN_MASK | (5 << CFGREG_COREFREQ_PLL_N_SHIFT) | CFGREG_COREFREQ_PLL_REF_SEL_MASK;
    while ((CFG_REGS_PTR->CFGREG_COREFREQ_STAT & CFGREG_COREFREQ_STAT_PLL_LOCK_MASK) == 0);

    /* Switch to PLL */
    CFG_REGS_PTR->CFGREG_UNLOCK = CFGREG_UNLOCK_DEF;
    CFG_REGS_PTR->CFGREG_COREFREQ_CLK = 2 << CFGREG_COREFREQ_CLK_CORE_SEL_SHIFT;

    /* Disable APB1 Bridge */
    AMBA_APB0_CFG_PTR->APB1_CFG &= ~AMBA_APB1_EN;

}

/**
 * @brief Initialize GNSS AFE
 */
void gnss_afe_init(void)
{

    /* Configure GNSSAFE1 */
    /* Configure PM */
    CFG_REGS_PTR->CFGREG_UNLOCK = CFGREG_UNLOCK_DEF;
    CFG_REGS_PTR->CFGREG_PM_CONF = CFGREG_PM_CONF_DEF | CFGREG_PM_CONF_BGVR_EN_MASK | CFGREG_PM_CONF_IREF_EN_MASK | CFGREG_PM_CONF_VREF_EN_MASK | CFGREG_PM_CONF_LDO_RF_EN_MASK | CFGREG_PM_CONF_LDO_APLL_EN_MASK | CFGREG_PM_CONF_LDO_DPLL_EN_MASK | CFGREG_PM_CONF_LDO_IF_EN_MASK | CFGREG_PM_CONF_LDO_ADC_EN_MASK;
    while((CFG_REGS_PTR->CFGREG_PM_STAT & CFGREG_PM_STAT_PWR_UP_MASK) == 0) ;
    CFG_REGS_PTR->CFGREG_UNLOCK = CFGREG_UNLOCK_DEF;
    CFG_REGS_PTR->CFGREG_PM_CONF |= CFGREG_PM_CONF_NRST_MASK;
    /* Configure LNA1 */
    CFG_REGS_PTR->CFGREG_UNLOCK = CFGREG_UNLOCK_DEF;
    CFG_REGS_PTR->CFGREG_LNA15_CONF = CFGREG_LNA15_CONF_DEF | CFGREG_LNA15_CONF_EN_MASK | CFGREG_LNA15_CONF_EN_L1_MASK | CFGREG_LNA15_CONF_EN_L5_MASK;
    /* Configure MIXER_BALUN1 */
    CFG_REGS_PTR->CFGREG_UNLOCK = CFGREG_UNLOCK_DEF;
    CFG_REGS_PTR->CFGREG_BALUN_MIXER1_CONF = CFGREG_BALUN_MIXER1_CONF_DEF | CFGREG_BALUN_MIXER1_CONF_BALUN_EN_MASK | CFGREG_BALUN_MIXER1_CONF_MIXER_EN_MASK;
    /* Configure PLL1 */
    CFG_REGS_PTR->CFGREG_UNLOCK = CFGREG_UNLOCK_DEF;
    CFG_REGS_PTR->CFGREG_PLL1_CONF  = CFGREG_PLL1_CONF_DEF | CFGREG_PLL1_CONF_EN_MASK;
    /* Configure IF1 */
    CFG_REGS_PTR->CFGREG_UNLOCK = CFGREG_UNLOCK_DEF;
    CFG_REGS_PTR->CFGREG_IF1_CONF  = CFGREG_IF1_CONF_DEF | CFGREG_IF1_CONF_EN_MASK | CFGREG_IF1_CONF_LPF_EN_MASK | CFGREG_IF1_CONF_PGA2_EN_MASK | CFGREG_IF1_CONF_PGA1_EN_MASK | CFGREG_IF1_CONF_PREAMP_EN_MASK;
    /* Configure ADC1 */
    CFG_REGS_PTR->CFGREG_UNLOCK = CFGREG_UNLOCK_DEF;
    CFG_REGS_PTR->CFGREG_ADC1_CONF  = CFGREG_ADC1_CONF_DEF | CFGREG_ADC1_CONF_SAH_EN_MASK | CFGREG_ADC1_CONF_ADC_EN_MASK;

    /* Configure GNSSAFE5 */
    /* Configure MIXER_BALUN5 */
    CFG_REGS_PTR->CFGREG_UNLOCK = CFGREG_UNLOCK_DEF;
    CFG_REGS_PTR->CFGREG_BALUN_MIXER5_CONF  = CFGREG_BALUN_MIXER5_CONF_DEF | CFGREG_BALUN_MIXER5_CONF_BALUN_EN_MASK | CFGREG_BALUN_MIXER5_CONF_MIXER_EN_MASK;
    /* Configure PLL5 */
    CFG_REGS_PTR->CFGREG_UNLOCK = CFGREG_UNLOCK_DEF;
    CFG_REGS_PTR->CFGREG_PLL5_CONF  = CFGREG_PLL5_CONF_DEF | CFGREG_PLL5_CONF_EN_MASK;
    /* Configure IF5 */
    CFG_REGS_PTR->CFGREG_UNLOCK = CFGREG_UNLOCK_DEF;
    CFG_REGS_PTR->CFGREG_IF5_CONF  = CFGREG_IF5_CONF_DEF | CFGREG_IF5_CONF_EN_MASK | CFGREG_IF5_CONF_LPF_EN_MASK | CFGREG_IF5_CONF_PGA2_EN_MASK | CFGREG_IF5_CONF_PGA1_EN_MASK | CFGREG_IF5_CONF_PREAMP_EN_MASK;
    /* Configure ADC5 */
    CFG_REGS_PTR->CFGREG_UNLOCK = CFGREG_UNLOCK_DEF;
    CFG_REGS_PTR->CFGREG_ADC5_CONF  = CFGREG_ADC5_CONF_DEF | CFGREG_ADC5_CONF_SAH_EN_MASK | CFGREG_ADC5_CONF_ADC_EN_MASK;

}

