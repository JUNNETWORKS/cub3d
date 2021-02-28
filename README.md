# Cub3d

## .cub ファイル

ゲームのマップやテクスチャ情報, ウィンドウサイズなどは .cub という拡張子のファイルに記載して, それをプログラムから読み取るような形になっています.

以下例

```

R 1920 1080

NO ./path_to_the_north_texture
SO ./path_to_the_south_texture
WE ./path_to_the_west_texture
EA ./path_to_the_east_texture
S ./path_to_the_sprite_texture

F 220,100,0
C 225,30,0

1111111111111111111111111
1000000000110000000000001
1011000001110000002000001
1001000000000000000000001
111111111011000001110000000000001
100000000011000001110111111111111
11110111111111011100000010001
11110111111111011101010010001
11000000110101011100000010001
10002000000000001100000010001
10000000000000001101010010001
11000001110101011111011110N0111
11110111 1110101 101111010001
11111111 1111111 111111111111

```

## XPM ファイルの作成方法

imagemagick を使って PNG や JPEG から XPM ファイルを作成することが出来る.

```bash
# 縦横比を維持する
convert -depth 8 -colors 16 -resize 64x64 ~/Pictures/icon/pikachu_square.jpg test.xpm
# 縦横比を維持しない
convert -depth 8 -colors 16 -resize 64x64! ~/Pictures/icon/pikachu_square.jpg test.xpm
```
