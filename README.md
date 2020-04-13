# nextion
dmonitor用 Nextion アドオンドライバー

JARL D-STAR Commitee 作成の dmonitor にアドオンとしてインストールすることにより Nextion でコントロール出来るようになります。

==過去に古いバージョンで下の git による作業をしたことの有る方は==
すでに nextion フォルダが有りますので
$ cd nextion
$ git pull
とするだけで最新版が差分で引っ張れます。
$ make
$ sudo mv nextion /usr/local/bin
==終了======================================================

今まで ZIP ファイルをダウンロードしていた方は、次のコマンドを順に実行してください。

nextionアドオンのコンパイルとインストール
$ sudo apt install git
$ git clone https://github.com/ytodo/nextion.git
$ cd nextion
$ make
$ sudo mv nextion /usr/local/bin

nextion.iniのインストール
$ sudo cp nextion.ini /etc/
$ sudo nano /etc/nextion.ini

自動起動にする場合
$ sudo nextion.service /etc/systemd/system/
$ sudo systemctl enable nextion.service
$ sudo reboot
次回からは、最初に書いた作業だけで終了します。
