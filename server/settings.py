import os

HOME_FOLDER = "C:\Users\lauro.gama\Documents\pessoal\dash\server"
#HOME_FOLDER  ="/home/pi/dash/"
CONFIG = {}
CONFIG['LOG_PATH'] = HOME_FOLDER + 'log.log'
CONFIG['SECRET_KEY'] = 'USE-YOUR-OWN-SECRET-KEY-DAMNIT'
CONFIG['APP_PORT'] = 8000
CONFIG['SSID'] = "Salcomp_sys"
CONFIG['PASSWORD'] = "salcomp@2016"
CONFIG['SQLITE_TEST_DB'] = 'sqlite:///' + HOME_FOLDER + '/dash.db'
