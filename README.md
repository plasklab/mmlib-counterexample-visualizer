# MMLib Counterexample Visualizer
本リポジトリでは，[「MMLib」](https://github.com/plasklab/mmlib)
の反例を可視化するソフトウェアを公開しています．
本ソフトウェアを使用することで，モデル上に「MMLib」の反例に沿ったステップ実行の様子を表示できます．
本ソフトウェアはEclipseのプラグインとして実装されています．
最新のソフトウェアは，「jp.ac.kochi-tech.info.pl.mmlib.viewer_1.0.0.201812140943.jar」です．

## 動作環境
- SPIN (6.4.6で動作を確認)
- Eclipse (Eclipse2018-09で動作を確認)

### 必須プラグイン
- org.eclipse.core.resources (3.13.100)
- org.eclipse.jface.text (3.14.0)
- org.eclipse.swtbot.go (2.7.0)
- org.eclipse.ui.editors (3.11.200)
- org.eclipse.ui.ide (3.14.100)


## 環境構築の例
MacOSでEclipse2018-09のEclipse IDE for Java Developersに本ソフトウェアを導入する手順は以下の通りです．
1. 公式サイトから[Eclipse IDE for Java Developers](https://www.eclipse.org/downloads/packages/release/2018-09/r/eclipse-ide-java-developers)をダウンロード・インストールする．
2. 必要なソフトウェアをインストールする．メニューバーから「Help」 -> 「Install New Software」の順に選択し，開かれたダイアログの「Work with」テキストボックスのプルダウンメニューから 「2018-09 - http\://download.eclipse.org/releases/2018-09」を選択する．
ダイアログの中央の表にダウンロード可能なソフトウェアの一覧が表示されるので，
  - SWTBot for Eclipse Testing
  - Eclipse Plug-in Development Environment  
  をインストールする．
3. Eclipseの「dropins」に本ソフトウェアを配置する．
MacにEclipse_2018-09をインストールした場合は，以下のディレクトリに配置する．
```
インストールしたディレクトリ/Applications/Eclipse_2018-09.app/Contents/Eclipse/dropins/
```
4. Eclipseを再起動する．

※SPINのパスが`/usr/local/bin/spin`でない場合には，別途パスを設定する必要があります．パスの設定は，Eclipseの環境設定の中の「MMLib Viewer」の項目で行います．


## 使用方法
### 必要なファイル
  - モデルのファイル
  - 「MMLib」のライブラリのファイル  
  ※本ソフトウェアを使用する場合は，本リポジトリの`library`ディレクトリ内のライブラリを使用する
  - trailファイル（反例ファイル）
  - 共有変数情報ファイル  
  モデルで使用する共有変数の情報を記述する．  
  ファイル名は「モデルのファイル名」，拡張子は「.in」．  
  例：モデルのファイルが：「model.pml」なら「model.in」  
  [専用の文法](#共有変数情報ファイルの文法)で記述する．

### 画面構成
  ![gamen](software.png)

本ソフトウェアは，ステップ実行を制御する操作ビューと，モデルを表示するモデルビュー，
共有変数の表を表示する共有変数ビューなどから成ります．
操作ビューについては，[使用手順](#使用手順)や[操作方法](#操作方法)を参照してください．
モデルビューには，ステップ毎に次の情報が表示されます．
- 実行位置  
各スレッドの実行位置（プログラムカウンタ）が，緑色の矢印で表示されます．
ステップが切り替わると，そのステップで実行されたスレッドの実行位置が表示されるように，
モデルビューの表示範囲が切り替わります．
- 未反映の命令  
各スレッドが実行した命令のうち，
共有変数へ結果が反映されていない命令のある行が黄色でハイライトされます．
共有変数に未反映の命令の結果は，実行したスレッドからしか観測できません．
- プログラム順序とメモリ順序が異なった命令  
命令が共有変数へ反映されたステップでは，未反映の命令のうち，
反映された命令よりも前に実行されていた命令のある行が赤色でハイライトされることがあります．
赤色でハイライトされる条件は次の通りです．
  - ある命令（`READ` or `WRITE`）が共有変数へ反映されたステップで，
  プログラム順序で先行するが共有変数へ未反映の`WRITE`
  - `READ`が共有変数に反映されたステップで，他のスレッドが実行した同じ共有変数を対象とする`WRITE`のうち，
  後続の命令（`READ` or `WRITE`）が共有変数へ反映されているもの  
  （一度でも赤色でハイライトされたもの）


### 使用手順
  1. MMLibを使用したモデルをモデル検査する．  
  ※本リポジトリの`library`ディレクトリ内のライブラリを使用する．
  1. Eclipseのワークスペースに各ファイルを同じディレクトリにインポートする．
  2. メニューバーから「Window」 -> 「Show View」 -> 「Other…」の順に選択し，表示されるダイアログ中の「General」フォルダの下の「MMLib Viewer」を選択する．共有変数の一覧を表示したい場合は「MMLib Viewer Shared Variables Table」を，ローカル変数の一覧を表示したい場合は「MMLib Viewer Local Variables Table」をそれぞれ選択する．
  3. 操作ビューのモデル選択ボタンをクリックし，プログラムのモデルを選択する．プログラムのモデルを選択せると，反例ファイルの一覧が自動的に表示される．
  4. 反例ファイルの一覧から，使用するファイルを選択する．
  ファイルを選択すると，反例は自動的に解析される．
  再読み込みしたい場合は「Reload」ボタンをクリックする．
  5. [各種ボタン](#操作方法)からステップ実行を操作する．

### チュートリアル
しばらくお待ちください．

### 操作方法
  - ナビゲーションボタン  
    - Begin  
    最初のステップに戻る
    - Prev Line  
    モデルの行で1行戻る
    - Prev  
    1ステップ戻る
    - Next  
    1ステップ進む
    - Next Line  
    モデルの行で1行進む
    - End  
    最後のステップに進む

### 共有変数情報ファイルの文法
```
<行>       ::= <element> at <location>
<element>  ::= <scalar> | <array> | <struct>
<scalar>   ::= <変数名>
<array>    ::= <element> [ <整数値> ]
<struct>   ::= <変数名> { <element>* }
<location> ::= <変数番号> | next <変数名>
```

各行に`<行>`を記述し，
`at`の左の`<element>`には`<変数名>`と`<構造>`を指定します．
共有変数がスカラの時は，`<変数名>`だけを記述します．
共有変数が配列の時は，配列の要素の指定の直後に`[3]`のように要素数を記述します．
共有変数が構造体の時は，`pair {x, y}`のように構造体名の後に要素を記述します．

`at`の右の`<location>`には`<変数番号>`を記述します．
MMLibの内部では共有変数は配列で管理されていて，`<変数番号>`はそのインデックスを表します．
`<location>`には，`<変数番号>`の代わりに他の共有変数の`<変数名>`を指定することで，その共有変数の直後の`<変数番号>`を指定できます．


###
