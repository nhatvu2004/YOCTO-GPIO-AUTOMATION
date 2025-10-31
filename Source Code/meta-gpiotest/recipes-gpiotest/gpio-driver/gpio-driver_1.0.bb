DESCRIPTION = "GPIO Driver for Raspberry Pi"
LICENSE = "GPL-2.0"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/GPL-2.0-only;md5=801f80980d171dd6425610833a22dbe6"

SRC_URI = "file://gpio-driver.c \
           file://Makefile \
           file://gpio-driver.service"

S = "${WORKDIR}"

inherit module
inherit systemd

SYSTEMD_SERVICE:${PN} = "gpio-driver.service"

do_compile() {
    oe_runmake
}

# Cài đặt module vào thư mục modules
do_install() {
    install -d ${D}${base_libdir}/modules/${KERNEL_VERSION}/extra
    install -m 0644 gpio-driver.ko ${D}${base_libdir}/modules/${KERNEL_VERSION}/extra
    
    install -d ${D}${systemd_system_unitdir}
    install -m 0644 gpio-driver.service ${D}${systemd_system_unitdir}
}


FILES_${PN} += "${BASE_LIBDIR}/modules/${KERNEL_VERSION}/extra/gpio-driver.ko \
		${systemd_system_unitdir}/gpio-driver.service"
		
		
