# XDFWRITE.X

XDF image writer for X680x0/Human68k

XDFイメージファイルを実機上で実FDに書き込みます。

---

## Install

XDFWRxxx.ZIP をダウンロードして展開し、XDFWRITE.X をパスの通ったディレクトリにコピーします。

---

## Usage

    usage: XDFWRITE <xdf-image-file> <drive-number>
    xdf-image-file ... XDF ファイル名 (1,261,568バイトであること)
    drive-number ... 0,1,2,3 のいずれか

FDは事前にフォーマットしておく必要があります。

---

## Notice

本ソフトウェアを利用したことにより、何らかの損害(データ消失、FDメディア破損、FDD故障など)が発生しても対応できませんので、自己責任でご利用ください。

---

### 参考: Mac で 3.5FD から XDF イメージを作る方法

* USB接続の3.5FDDを用意して繋ぐ。

* 1.2MB 2HD フォーマットの 3.5FD を入れる。

* 扱えないフォーマット云々の警告がでるが無視する。

* macOS 標準のディスクユーティリティで ファイル -> 新規イメージ -> (USB FDD名)からイメージを作成 を選択してイメージファイル(.dmg)を作る

* .dmgの最後尾には管理情報が含まれているため、Terminal を開いて head コマンドで後ろをカットする。

    head -c 1261568 hogehoge.dmg > hogehoge.xdf

以上
