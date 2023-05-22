# linux

remove mysql
sudo systemctl disable mysql
sudo apt remove mysql*
sudo apt purge mysql*
sudo apt autoremove

sudo rm -r /etc/mysql /var/lib/mysql /var/log/mysql

whereis mysql


nano /etc/systemd/system/mysql.service.d/override.conf
