# nextion
dmonitor用 Nextion アドオンドライバー

JARL D-STAR Commitee 作成の dmonitor にアドオンとしてインストールすることにより Nextion でコントロール出来るようになります。

==過去に古いバージョンで下の git による作業をしたことの有る方は==<br>
すでに nextion フォルダが有りますので<br>
$ cd nextion<br>
$ git pull<br>
とするだけで最新版が差分で引っ張れます。<br>
$ make<br>
$ sudo make install<br>
==終了======================================================<br>

今まで ZIP ファイルをダウンロードしていた方は、次のコマンドを順に実行してください。<br>

nextionアドオンのコンパイルとインストール<br>
$ sudo apt install git<br>
$ git clone https://github.com/ytodo/nextion.git<br>
$ cd nextion<br>
$ make<br>
$ sudo make install<br>

nextion.iniのインストール<br>
$ sudo cp nextion.ini /etc/<br>
$ sudo nano /etc/nextion.ini<br>

nextion.iniの説明を読んで、お使いになる方の環境に合わせてください。<br>

自動起動にする場合<br>
$ sudo nextion.service /etc/systemd/system/<br>
$ sudo systemctl enable nextion.service<br>
$ sudo reboot<br>
もし、タイミングによって立ち上がらない場合は、nextion.timerの秒数を調整して<br>
$ sudo systemctl enable nextion.timer<br>
$ sudo systemctl disable nextion.service<br>
としてください。timerを有効にした場合は、serviceを無効にすることに注意してください。<br>

次回からは、最初に書いた作業だけで終了します。<br>
