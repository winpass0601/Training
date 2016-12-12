・操作方法
　-Spaceキー　球を突く
  
  -pushVectorウィンドウの値をドラッグして発射速度、発射方向を調整できます。
	（方向とかグラフィカルに見れなくて申し訳ないです・・・）




・DirectX
  -DirectXフォルダ内にあるファイルで動くと思います。
　 動かない場合は、DirectX\Redist\DXSETUP.exeを実行してください。

・DirectXTex
  -プロジェクトに同梱しました。特に設定無しで動くと思います。
   動かない場合はプロジェクトのプロパティから 
   
   C/C++ > 追加のインクルードディレクトリで DirectXTexフォルダを設定。
   リンカー > 追加のライブラリディレクトリで DirectXTex\lib\$(PlatformTarget)\(configuration)を設定。
　 
   してください。

・boost
  -ビルド時にnugetで自動ダウンロードされます。