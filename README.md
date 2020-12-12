# nextion
dmonitor用 Nextion アドオンドライバー

JARL D-STAR Commitee 作成の dmonitor にアドオンとしてインストールすることにより Nextion でコントロール出来るようになります。

==過去に古いバージョンで下の git による作業をしたことの有る方は==<br>
すでに nextion フォルダが有りますので<br>
$ cd nextion<br>
$ git pull<br>
とするだけで最新版が差分で引っ張れます。<br>
$ sudo make update<br>
<b>上記のアップデートコマンドでは、設定ファイルが上書きされることは有りません</b>
但し、1.6.5-->1.6.6 では、nextion.iniの項目が増えていますので、編集するか置き換えてください。

==終了======================================================<br>

今まで ZIP ファイルをダウンロードしていた方は、次のコマンドを順に実行してください。<br>

nextionアドオンのコンパイルとインストール<br>
$ sudo apt install git<br>
$ git clone https://github.com/ytodo/nextion.git<br>
$ cd nextion<br>
$ sudo make install<br>

nextion.iniの編集<br>
$ sudo nano /etc/nextion.ini<br>

nextion.iniの説明を読んで、お使いになる方の環境に合わせてください。<br>

自動起動にする場合<br>
上記インストール手順によって自動的に設定されます。
もし、タイミングによって立ち上がらない場合は、nextion.timerの秒数を調整して<br>
$ sudo systemctl enable nextion.timer<br>
$ sudo systemctl disable nextion.service<br>
としてください。timerを有効にした場合は、serviceを無効にすることに注意してください。<br>

次回からは、最初に書いた作業だけで終了します。<br>
