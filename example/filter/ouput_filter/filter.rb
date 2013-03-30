# httpd.conf 設定
#
# SetOutputFilter mruby
# mrubyOutputFilter /tmp/filter.rb
#

f = Apache::Filter.new

# brigade のデータ取得
data = f.flatten

# データ取得したので元のbrigadeは綺麗に
f.cleanup

# 綺麗にしたbigaedeに弄ったbrigadeデータを書き込み
f.insert_tail "inset_tail したよ #{data * 3}"

# streamの終了を明示
f.insert_eos
