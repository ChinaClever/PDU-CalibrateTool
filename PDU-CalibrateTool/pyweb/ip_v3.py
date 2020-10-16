from pyweb.ip_web import  *

class IpV3(IpWeb):

    def start_fun(self):
        self.login()
        self.setCorrect()
        self.setEle()
        self.setThreshold()
        self.clearLogs()
        self.driver.close()

    def setTime(self):
        self.divClick(8)
        self.execJsAlert("check_time()")

    def clearLogs(self):
        en = self.cfgs['log_en']
        if(int(en) < 1):
            return
        self.setTime()
        self.divClick(9)
        for num in range(0, 2):
            self.setSelect("loglist", num)
            self.execJsAlert("funClearRecords()")

    def setCorrect(self):
        cfg = self.cfgs
        ip = 'http://' + cfg['ip'] + '/correct.html'
        self.driver.get(ip); time.sleep(1)
        self.driver.switch_to.default_content()
        self.setItById("language", cfg['ip_language'])
        self.setItById("modbus", cfg['ip_modbus'])
        self.setItById("lcdled", cfg['ip_lines'])
        self.setItById("standard", cfg['ip_standard'])
        self.setItById("LCDswitch", cfg['lcd_switch'])
        self.setItById("Log_flag", cfg['log_en'])
        if (len(cfg['mac']) > 5):
            self.setItById("mac1", cfg['mac'])
        self.alertClick("Button3")
        self.driver.back(); time.sleep(1)





