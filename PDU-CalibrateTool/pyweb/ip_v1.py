from pyweb.ip_web import  *

class IpV1(IpWeb):

    def start_fun(self):
        self.login()
        self.setCorrect()
        self.setEle()
        self.setThreshold()
        time.sleep(1)
        self.driver.close()

    def setCorrect(self):
        cfg = self.cfgs
        ip = 'http://' + cfg['ip'] + '/correct.html'
        self.driver.get(ip)
        time.sleep(1)
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
        
    def setEle(self):
        self.divClick(3)
        line = int(self.cfgs['ip_lines'])
        jsSheet = " claerset = createXmlRequest();claerset.onreadystatechange = clearrec;ajaxget(claerset, \"/energyzero?a=\" + {0}+\"&\");"
        
        if('直流电能:' == self.driver.find_element_by_id("L4").text):
            self.execJs(jsSheet.format(3))
        else:
            for num in range(0, line):
                self.execJs(jsSheet.format(num))
    
    








