SUMMARY = "GPIO Test Application"
DESCRIPTION = "Automated GPIO testing application for Raspberry Pi 4"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

SRC_URI = "file://gpio-test.c \
	   file://gpio-test.service"

S = "${WORKDIR}"

inherit systemd
SYSTEMD_SERVICE:${PN} = "gpio-test.service"

do_compile() {
    ${CC} gpio-test.c ${LDFLAGS} -o gpio-test 
}

do_install() {
    install -d ${D}${bindir}
    install -m 0755 gpio-test ${D}${bindir}

    install -d ${D}${systemd_system_unitdir}
    install -m 0644 gpio-test.service ${D}${systemd_system_unitdir}
}

FILES_${PN} += "${bindir}/gpio-test \
		${systemd_system_unitdir}/gpio-test.service"              

               
