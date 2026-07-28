from ftplib import FTP

ftp = FTP()

ftp.connect(host='192.168.1.31',port=8179) 
ftp.login(user='pc',passwd='5276')

ftp.connect()

ftp.nlst()
