# linux

remove mysql
sudo systemctl disable mysql
sudo apt remove mysql*
sudo apt purge mysql*
sudo apt autoremove

sudo rm -r /etc/mysql /var/lib/mysql /var/log/mysql

whereis mysql


nano /etc/systemd/system/mysql.service.d/override.conf

[Service]
ExecStart=
ExecStart=/usr/sbin/mysqld --skip-grant-tables --skip-networking #untuk reset password

systemctl restart mysql

mysql

flush privileges;

ALTER USER 'root'@'localhost' IDENTIFIED WITH mysql_native_password BY 'password';

flush privileges;

edit kembali override.conf 

restart mysql


