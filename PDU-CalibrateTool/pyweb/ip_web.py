from selenium import webdriver
from selenium.webdriver.support.select import Select
import configparser
import time
import os

class IpWeb:

    def __init__(self):
        self.initCfg()
        self.initDriver()

    def initDriver(self):
        try:
            self.driver = webdriver.Firefox()
        except ValueError:
            self.driver = webdriver.Chrome()

    @staticmethod
    def getCfg():
        cf = configparser.ConfigParser()
        fn = os.path.expanduser('~') + "/.PDU-CalibrateTool/cfg.ini"
        cf.read(fn)  # 读取配置文件，如果写文件的绝对路径，就可以不用os模块
        #cf.read("cfg.ini")
        return cf

    def initCfg(self):
        items = IpWeb.getCfg().items("Cfg")  # 获取section名为Mysql-Database所对应的全部键值对
        self.cfgs = {'ip_version':1,'user': 'admin', 'pwd': 'admin',
                     'ip': '192.168.1.163', 'debug_web':  'correct.html',
                     'ip_lines':1, 'ip_modbus':1, 'ip_language':1, 'lcd_switch':1,
                     'mac':'', 'log_en':1, 'ip_standard': 0}
        for it in items:
            self.cfgs[it[0]] = it[1]

    def login(self):
        ip =  'http://'+self.cfgs['ip']+'/'
        user = self.cfgs['user']
        pwd = self.cfgs['pwd']
        self.driver.get(ip)
        self.setItById("name", user)
        self.setItById("psd", pwd)
        self.execJs("login()")

    def setCur(self, lines, min, max):
        for num in range(1, lines+1):
            self.setItById("min" + str(num), int(min)/10)
            self.setItById("max" + str(num), int(max)/10)
            self.execJs("setlimit({0})".format(num))

    def setVol(self, lines, min, max):
        for num in range(4, lines+4):
            self.setItById("min" + str(num), min)
            self.setItById("max" + str(num), max)
            self.execJs("setlimit({0})".format(num))

    def setEnv(self):
        self.setItById("min7", 0)
        self.setItById("max7", 60)
        self.execJs("setlimit(7)")
        self.setItById("min8", 0)
        self.setItById("max8", 90)
        self.execJs("setlimit(8)")

    def setThreshold(self):
        curMin = self.cfgs['cur_min']
        curMax = self.cfgs['cur_max']
        volMin = self.cfgs['vol_min']
        volMax = self.cfgs['vol_max']
        line = int(self.cfgs['ip_lines'])
        self.divClick(2)
        self.setCur(line, curMin, curMax)
        self.setVol(line, volMin, volMax)
        self.setEnv()

    def setEle(self):
        self.divClick(3)
        line = int(self.cfgs['ip_lines'])
        for num in range(0, line):
            self.execJsAlert("energyclaer({0})".format(num))

    def setSelect(self, id, v):
        it = self.driver.find_element_by_id(id)
        Select(it).select_by_index(v)
        time.sleep(1)

    def setItById(self, id, v):
        it = self.driver.find_element_by_id(id)
        it.clear()
        it.send_keys(str(v))
        time.sleep(1)

    def btnClick(self, id):
        self.driver.find_element_by_id(id).click()
        time.sleep(1)

    def alertClick(self, id):
        self.btnClick(id)
        self.driver.switch_to.alert.accept()
        time.sleep(1)

    def divClick(self, id):
        self.driver.switch_to.default_content()
        self.execJs("clk({0})".format(id))
        self.driver.switch_to.frame('ifrm')

    def execJs(self, js):
        self.driver.execute_script(js)
        time.sleep(1)

    def execJsAlert(self, js):
        self.execJs(js)
        self.driver.switch_to.alert.accept()
        time.sleep(1)




