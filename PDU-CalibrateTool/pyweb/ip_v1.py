from pyweb.ip_web import  *

class IpV1(IpWeb):

    def start_fun(self):
        self.login()
        self.setCorrect()
        self.setEle()
        self.setThreshold()
        time.sleep(1)
        self.resetFactory()
        self.driver.close()

    def setCorrect(self):
        cfg = self.cfgs
        ip =  self.ip_prefix + cfg['ip'] + '/correct.html'
        self.driver.get(ip); time.sleep(1)
        self.driver.switch_to.default_content()
        self.setItById("language", int(cfg['ip_language'])+1)
        self.setItById("modbus", cfg['ip_modbus'])
        self.setItById("lcdled", cfg['ip_lines'])
        self.setItById("horizontal", cfg['lcd_switch'])
        if(len(cfg['mac']) > 5) :
            self.setItById("mac1", cfg['mac'])
        self.alertClick("Button3")
        self.driver.back()
        time.sleep(1)
    
    








