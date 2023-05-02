# goblib_m5s
STL を使用した M5Stack に依存したライブラリです。

## 概要
C++11 以降をコンパイルできる環境下での使用を想定しています。

## 必要なもの
[M5Stack](https://github.com/m5stack/M5Stack) 0.4.3 or later  
[LovyanGFX](https://github.com/lovyan03/LovyanGFX) 1.1.6 or later  
[M5GFX](https://github.com/m5stack/M5GFX) 0.1.6 or later  
[SdFat](https://github.com/greiman/SdFat) 2.2.2 or later  
[goblib](https://github.com/GOB52/goblib) 0.2.0 or later  

**LovyanGFX と M5GFX は排他的です。どちらかを使用してください。**


## インストール
各環境の適切な位置に git clone するか、ZIP をダウンロード後展開して下さい。  
コンパイラに当該位置を示すオプション追加が必要かどうかは開発環境によります。

## ドキュメントの作成方法
[Doxygen](http://www.doxygen.jp/) によりライブラリドキュメントを生成できます。 [Doxygen設定ファイル](doc/Doxyfile)  
出力する際に [doxy.sh](doc/doxy.sh) を使用すると、library.properties からVersion、repository から rev を取得し、Doxygen 出力できます。

