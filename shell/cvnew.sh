#!/bin/sh

if [ $# -ne 2 ]; then
	echo "Usage $0 d31csv hh:mm"
	exit 1
fi

d31_file="$1"
chk_time="$2"

tmp_time=`echo $chk_time | sed  's/.*\([0-2][0-9]\:[0-5][0-9]\).*/\1/'`
tmp_time=${tmp_time:-"0"}

if [ "$chk_time" != "$tmp_time" ];then
	echo "param2=$chk_time error format is hh:mm e.g. 09:30"
	echo "Usage $0 d31csv hh:mm"
	exit 1;
fi

[ ! -f $d31_file ] && echo "$d31_file is not exist" && exit 1;

stat_info=`sed -n "/$chk_time/,+p" $d31_file`

TradeTime=`echo $stat_info | awk -F, '{print $1 	}'`
zb_a_0=`echo $stat_info | awk -F, '{print $2 	}'`
zb_a_5=`echo $stat_info | awk -F, '{print $3 	}'`
zb_a_10=`echo $stat_info | awk -F, '{print $4 	}'`
zb_a_20=`echo $stat_info | awk -F, '{print $5 	}'`
zb_a_40=`echo $stat_info | awk -F, '{print $6 	}'`
zb_a_60=`echo $stat_info | awk -F, '{print $7 	}'`
zb_a_80=`echo $stat_info | awk -F, '{print $8 	}'`
zb_a_100=`echo $stat_info | awk -F, '{print $9 	}'`
zb_a_200=`echo $stat_info | awk -F, '{print $10  }'`
zb_a_500=`echo $stat_info | awk -F, '{print $11  }'`
zb_v_0=`echo $stat_info | awk -F, '{print $12  }'`
zb_v_5=`echo $stat_info | awk -F, '{print $13  }'`
zb_v_10=`echo $stat_info | awk -F, '{print $14  }'`
zb_v_20=`echo $stat_info | awk -F, '{print $15  }'`
zb_v_40=`echo $stat_info | awk -F, '{print $16  }'`
zb_v_60=`echo $stat_info | awk -F, '{print $17  }'`
zb_v_80=`echo $stat_info | awk -F, '{print $18  }'`
zb_v_100=`echo $stat_info | awk -F, '{print $19  }'`
zb_v_200=`echo $stat_info | awk -F, '{print $20  }'`
zb_v_500=`echo $stat_info | awk -F, '{print $21  }'`
zb_n_0=`echo $stat_info | awk -F, '{print $22  }'`
zb_n_5=`echo $stat_info | awk -F, '{print $23  }'`
zb_n_10=`echo $stat_info | awk -F, '{print $24  }'`
zb_n_20=`echo $stat_info | awk -F, '{print $25  }'`
zb_n_40=`echo $stat_info | awk -F, '{print $26  }'`
zb_n_60=`echo $stat_info | awk -F, '{print $27  }'`
zb_n_80=`echo $stat_info | awk -F, '{print $28  }'`
zb_n_100=`echo $stat_info | awk -F, '{print $29  }'`
zb_n_200=`echo $stat_info | awk -F, '{print $30  }'`
zb_n_500=`echo $stat_info | awk -F, '{print $31  }'`
zs_a_0=`echo $stat_info | awk -F, '{print $32  }'`
zs_a_5=`echo $stat_info | awk -F, '{print $33  }'`
zs_a_10=`echo $stat_info | awk -F, '{print $34  }'`
zs_a_20=`echo $stat_info | awk -F, '{print $35  }'`
zs_a_40=`echo $stat_info | awk -F, '{print $36  }'`
zs_a_60=`echo $stat_info | awk -F, '{print $37  }'`
zs_a_80=`echo $stat_info | awk -F, '{print $38  }'`
zs_a_100=`echo $stat_info | awk -F, '{print $39  }'`
zs_a_200=`echo $stat_info | awk -F, '{print $40  }'`
zs_a_500=`echo $stat_info | awk -F, '{print $41  }'`
zs_v_0=`echo $stat_info | awk -F, '{print $42  }'`
zs_v_5=`echo $stat_info | awk -F, '{print $43  }'`
zs_v_10=`echo $stat_info | awk -F, '{print $44  }'`
zs_v_20=`echo $stat_info | awk -F, '{print $45  }'`
zs_v_40=`echo $stat_info | awk -F, '{print $46  }'`
zs_v_60=`echo $stat_info | awk -F, '{print $47  }'`
zs_v_80=`echo $stat_info | awk -F, '{print $48  }'`
zs_v_100=`echo $stat_info | awk -F, '{print $49  }'`
zs_v_200=`echo $stat_info | awk -F, '{print $50  }'`
zs_v_500=`echo $stat_info | awk -F, '{print $51  }'`
zs_n_0=`echo $stat_info | awk -F, '{print $52  }'`
zs_n_5=`echo $stat_info | awk -F, '{print $53  }'`
zs_n_10=`echo $stat_info | awk -F, '{print $54  }'`
zs_n_20=`echo $stat_info | awk -F, '{print $55  }'`
zs_n_40=`echo $stat_info | awk -F, '{print $56  }'`
zs_n_60=`echo $stat_info | awk -F, '{print $57  }'`
zs_n_80=`echo $stat_info | awk -F, '{print $58  }'`
zs_n_100=`echo $stat_info | awk -F, '{print $59  }'`
zs_n_200=`echo $stat_info | awk -F, '{print $60  }'`
zs_n_500=`echo $stat_info | awk -F, '{print $61  }'`
wb_a_0=`echo $stat_info | awk -F, '{print $62  }'`
wb_a_5=`echo $stat_info | awk -F, '{print $63  }'`
wb_a_10=`echo $stat_info | awk -F, '{print $64  }'`
wb_a_20=`echo $stat_info | awk -F, '{print $65  }'`
wb_a_40=`echo $stat_info | awk -F, '{print $66  }'`
wb_a_60=`echo $stat_info | awk -F, '{print $67  }'`
wb_a_80=`echo $stat_info | awk -F, '{print $68  }'`
wb_a_100=`echo $stat_info | awk -F, '{print $69  }'`
wb_a_200=`echo $stat_info | awk -F, '{print $70  }'`
wb_a_500=`echo $stat_info | awk -F, '{print $71  }'`
wb_v_0=`echo $stat_info | awk -F, '{print $72  }'`
wb_v_5=`echo $stat_info | awk -F, '{print $73  }'`
wb_v_10=`echo $stat_info | awk -F, '{print $74  }'`
wb_v_20=`echo $stat_info | awk -F, '{print $75  }'`
wb_v_40=`echo $stat_info | awk -F, '{print $76  }'`
wb_v_60=`echo $stat_info | awk -F, '{print $77  }'`
wb_v_80=`echo $stat_info | awk -F, '{print $78  }'`
wb_v_100=`echo $stat_info | awk -F, '{print $79  }'`
wb_v_200=`echo $stat_info | awk -F, '{print $80  }'`
wb_v_500=`echo $stat_info | awk -F, '{print $81  }'`
wb_n_0=`echo $stat_info | awk -F, '{print $82  }'`
wb_n_5=`echo $stat_info | awk -F, '{print $83  }'`
wb_n_10=`echo $stat_info | awk -F, '{print $84  }'`
wb_n_20=`echo $stat_info | awk -F, '{print $85  }'`
wb_n_40=`echo $stat_info | awk -F, '{print $86  }'`
wb_n_60=`echo $stat_info | awk -F, '{print $87  }'`
wb_n_80=`echo $stat_info | awk -F, '{print $88  }'`
wb_n_100=`echo $stat_info | awk -F, '{print $89  }'`
wb_n_200=`echo $stat_info | awk -F, '{print $90  }'`
wb_n_500=`echo $stat_info | awk -F, '{print $91  }'`
ws_a_0=`echo $stat_info | awk -F, '{print $92  }'`
ws_a_5=`echo $stat_info | awk -F, '{print $93  }'`
ws_a_10=`echo $stat_info | awk -F, '{print $94  }'`
ws_a_20=`echo $stat_info | awk -F, '{print $95  }'`
ws_a_40=`echo $stat_info | awk -F, '{print $96  }'`
ws_a_60=`echo $stat_info | awk -F, '{print $97  }'`
ws_a_80=`echo $stat_info | awk -F, '{print $98  }'`
ws_a_100=`echo $stat_info | awk -F, '{print $99  }'`
ws_a_200=`echo $stat_info | awk -F, '{print $100 }'`
ws_a_500=`echo $stat_info | awk -F, '{print $101 }'`
ws_v_0=`echo $stat_info | awk -F, '{print $102 }'`
ws_v_5=`echo $stat_info | awk -F, '{print $103 }'`
ws_v_10=`echo $stat_info | awk -F, '{print $104 }'`
ws_v_20=`echo $stat_info | awk -F, '{print $105 }'`
ws_v_40=`echo $stat_info | awk -F, '{print $106 }'`
ws_v_60=`echo $stat_info | awk -F, '{print $107 }'`
ws_v_80=`echo $stat_info | awk -F, '{print $108 }'`
ws_v_100=`echo $stat_info | awk -F, '{print $109 }'`
ws_v_200=`echo $stat_info | awk -F, '{print $110 }'`
ws_v_500=`echo $stat_info | awk -F, '{print $111 }'`
ws_n_0=`echo $stat_info | awk -F, '{print $112 }'`
ws_n_5=`echo $stat_info | awk -F, '{print $113 }'`
ws_n_10=`echo $stat_info | awk -F, '{print $114 }'`
ws_n_20=`echo $stat_info | awk -F, '{print $115 }'`
ws_n_40=`echo $stat_info | awk -F, '{print $116 }'`
ws_n_60=`echo $stat_info | awk -F, '{print $117 }'`
ws_n_80=`echo $stat_info | awk -F, '{print $118 }'`
ws_n_100=`echo $stat_info | awk -F, '{print $119 }'`
ws_n_200=`echo $stat_info | awk -F, '{print $120 }'`
ws_n_500=`echo $stat_info | awk -F, '{print $121 }'`
sd_bv=`echo $stat_info | awk -F, '{print $122 }'`
sd_av=`echo $stat_info | awk -F, '{print $123 }'`
sd_bm=`echo $stat_info | awk -F, '{print $124 }'`
sd_am=`echo $stat_info | awk -F, '{print $125 }'`
jb_tv=`echo $stat_info | awk -F, '{print $126 }'`
ja_tv=`echo $stat_info | awk -F, '{print $127 }'`
jb_tm=`echo $stat_info | awk -F, '{print $128 }'`
ja_tm=`echo $stat_info | awk -F, '{print $129 }'`
avg_bp=`echo $stat_info | awk -F, '{print $130 }'`
avg_ap=`echo $stat_info | awk -F, '{print $131 }'`
last_close=`echo $stat_info | awk -F, '{print $132 }'`
new_price=`echo $stat_info | awk -F, '{print $133 }'`
avg_bm=`echo $stat_info | awk -F, '{print $134 }'`
avg_am=`echo $stat_info | awk -F, '{print $135 }'`
jmp_b20=`echo $stat_info | awk -F, '{print $136 }'`
jmp_a20=`echo $stat_info | awk -F, '{print $137 }'`
jmp_b50=`echo $stat_info | awk -F, '{print $138 }'`
jmp_a50=`echo $stat_info | awk -F, '{print $139 }'`
jmp_b100=`echo $stat_info | awk -F, '{print $140 }'`
jmp_a100=`echo $stat_info | awk -F, '{print $141 }'`

##获取证券代码、小时、分钟信息
##sz_code=`echo $d31_file | cut -c 9-14`
sz_code=${d31_file:8:6}

##time_hh=`echo $chk_time | cut -c 1-2`
##time_mm=`echo $chk_time | cut -c 4-5`
time_hh=${chk_time:0:2}
time_mm=${chk_time:3:2}
	
tmp_time=`echo $time_hh*100+$time_mm | bc`

#printf "code=%06.0f,t=%02s%02s00000,zd,zb\n" $sz_code $time_hh $time_mm

declare -a arr_level

arr_level=(0 5 10 20 40 60 80 100 200 500)

frag_bid_amnt="b_a_"
frag_bid_volume="b_v_"
frag_bid_num="b_n_"
frag_ask_amnt="s_a_"
frag_ask_volume="s_v_"
frag_ask_num="s_n_"

var_bid_amnt=
var_bid_volume=
var_bid_num=
var_ask_amnt=
var_ask_volume=
var_ask_num=

prefix=""
prename=""

for prefix in "z" "w"
do 
	if [ $prefix = "z" ];then
		prename="zb"
	else
		prename="zd"
	fi

	for(( i=0;i<${#arr_level[@]};i++))
	do
		var_bid_amnt=$prefix$frag_bid_amnt${arr_level[i]}
		var_bid_volume=$prefix$frag_bid_volume${arr_level[i]}
		var_bid_num=$prefix$frag_bid_num${arr_level[i]}
		var_ask_amnt=$prefix$frag_ask_amnt${arr_level[i]}
		var_ask_volume=$prefix$frag_ask_volume${arr_level[i]}
		var_ask_num=$prefix$frag_ask_num${arr_level[i]}
        	
#		echo $var_bid_amnt $var_bid_volume $var_bid_num $var_ask_amnt $var_ask_volume $var_ask_num
##		echo $(eval echo $var_bid_amnt)*1000000+0.5 | bc
        	
		bid_amnt=`eval echo '$'"$var_bid_amnt"`
		bid_amnt=`echo $bid_amnt*1000000+0.5 | bc`
		ask_amnt=`eval echo '$'"$var_ask_amnt"`
		ask_amnt=`echo $ask_amnt*1000000+0.5 | bc`
		
		bid_volume=`eval echo '$'"$var_bid_volume"`
		bid_volume=`echo $bid_volume*100+0.5 | bc`
		ask_volume=`eval echo '$'"$var_ask_volume"`
		ask_volume=`echo $ask_volume*100+0.5 | bc`
		
		bid_num=`eval echo '$'"$var_bid_num"`
		bid_num=`echo $bid_num+0.5 | bc`
		ask_num=`eval echo '$'"$var_ask_num"`
		ask_num=`echo $ask_num+0.5 | bc`

#		echo $i=${arr_level[i]} $var_bid_amnt $var_bid_volume $var_bid_num $var_ask_amnt $var_ask_volume $var_ask_num
#		echo $i=${arr_level[i]} $bid_amnt $bid_volume $bid_num $ask_amnt $ask_volume $ask_num
	
#		printf "%s:i=%-5d\tbm=%-10.0f\tbv=%-6.0f\tbo=%-6.0f\tam=%-10.0f\tav=%-6.0f\tao=%-6.0f\n" $prename ${arr_level[i]} $bid_amnt $bid_volume $bid_num $ask_amnt $ask_volume $ask_num

#		printf "%-10.0f,%-6.0f,%-6.0f,%-10.0f,%-6.0f,%-6.0f\n" $bid_amnt $bid_volume $bid_num $ask_amnt $ask_volume $ask_num
		printf "%-6.0f,%s,%-4.0f,%-4.0f,%-10.0f,%-6.0f,%-6.0f,%-10.0f,%-6.0f,%-6.0f\n" $sz_code $prefix $tmp_time ${arr_level[i]} $bid_amnt $bid_volume $bid_num $ask_amnt $ask_volume $ask_num
	done

done

exit 0
