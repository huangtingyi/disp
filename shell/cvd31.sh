#!/bin/sh

if [ $# -ne 2 ]; then
	echo "Usage $0 d31csv hh:mm"
	exit 1
fi

d31_file="$1"
chk_time="$2"

##echo $chk_time | sed -n '/^[0-9]+1?:[0-9]+1$/,+p'
##
##if [ "X$chk_time" != "X`echo $chk_time | sed -n '/^[0-9]*:[0-9]*$/,+p'`" ];then
##	echo "param2=$chk_time error format is hh:mm e.g. 09:30"
##	echo "Usage $0 d31csv hh:mm"
##	exit 1;
##fi

[ ! -f $d31_file ] && echo "$d31_file is not exist" && exit 1;

stat_info=`sed -n "/2018-04-17 $chk_time/,+p" $d31_file`

#echo $stat_info | awk -F, '{print $1 	}'

#TradeTime=`echo $stat_info | awk -F, '{print $1 	}'`

#echo TradeTime=$TradeTime

#exit 1;

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


#code=603912,t=93000000,zd,zb
d31_base_file=`basename $d31_file`
sz_code=`echo $d31_base_file | cut -c 9-14`
#sz_code=${sz_code##*0}
#s/[0-9]*/-p&/g
sz_code=`echo $sz_code | sed 's/^0*//'`
#seq=`echo $filename | cut -c $seqbegin-$seqend`
time_hh=`echo $chk_time | cut -c 1-2`
time_mm=`echo $chk_time | cut -c 4-5`


echo zb_a_0=$zb_a_0 zb_v_0=$zb_v_0 code=$sz_code chk-time=$chk_time

printf "code=%06d,t=%02s%02s00000,zd,zb\n" $sz_code $time_hh $time_mm

#	pD31->alBidAmount[i],pD31->aiBidVolume[i],pD31->aiBidOrderNum[i],
#	pD31->alAskAmount[i],pD31->aiAskVolume[i],pD31->aiAskOrderNum[i]);

#bid_amnt=251358413
#bid_volume=94177
#bid_ordernum=22
#ask_amnt=56582800
#ask_volume=21200
#ask_ordernum=14


#printf "zb:i=%-5ld\tbm=%-10ld\tbv=%-6d\tbo=%d\tam=%-10ld\tav=%-6d\tao=%d\n" 0   $bid_amnt $bid_volume $bid_ordernum $ask_amnt $ask_volume $ask_ordernum

zb_a_0=`echo $zb_a_0*1000000+0.5 | bc`
zb_a_0=${zb_a_0%.*}

zb_v_0=`echo $zb_v_0*100+0.5 | bc`
zb_v_0=${zb_v_0%.*}

zb_n_0=`echo $zb_n_0+0.5 | bc`
zb_n_0=${zb_n_0%.*}

zs_a_0=`echo $zs_a_0*1000000+0.5 | bc`
zs_a_0=${zs_a_0%.*}

zs_v_0=`echo $zs_v_0*100+0.5 | bc`
zs_v_0=${zs_v_0%.*}

zs_n_0=`echo $zs_n_0+0.5 | bc`
zs_n_0=${zs_n_0%.*}

bid_amnt=$zb_a_0
bid_volume=$zb_v_0
bid_ordernum=$zb_n_0

ask_amnt=$zs_a_0
ask_volume=$zs_v_0
ask_ordernum=$zs_n_0

bid_amnt=${bid_amnt:-0}
bid_volume=${bid_volume:-0}
bid_ordernum=${bid_ordernum:-0}
ask_amnt=${ask_amnt:-0}
ask_volume=${ask_volume:-0}
ask_ordernum=${ask_ordernum:-0}

printf "zb:i=%-5ld\tbm=%-10ld\tbv=%-6d\tbo=%d\tam=%-10ld\tav=%-6d\tao=%d\n" 0   $bid_amnt $bid_volume $bid_ordernum $ask_amnt $ask_volume $ask_ordernum

zb_a_5=`echo $zb_a_5*1000000+0.5 | bc`
zb_a_5=${zb_a_5%.*}

zb_v_5=`echo $zb_v_5*100+0.5 | bc`
zb_v_5=${zb_v_5%.*}

zb_n_5=`echo $zb_n_5+0.5 | bc`
zb_n_5=${zb_n_5%.*}

zs_a_5=`echo $zs_a_5*1000000+0.5 | bc`
zs_a_5=${zs_a_5%.*}

zs_v_5=`echo $zs_v_5*100+0.5 | bc`
zs_v_5=${zs_v_5%.*}

zs_n_5=`echo $zs_n_5+0.5 | bc`
zs_n_5=${zs_n_5%.*}

bid_amnt=$zb_a_5
bid_volume=$zb_v_5
bid_ordernum=$zb_n_5

ask_amnt=$zs_a_5
ask_volume=$zs_v_5
ask_ordernum=$zs_n_5

bid_amnt=${bid_amnt:-0}
bid_volume=${bid_volume:-0}
bid_ordernum=${bid_ordernum:-0}
ask_amnt=${ask_amnt:-0}
ask_volume=${ask_volume:-0}
ask_ordernum=${ask_ordernum:-0}

printf "zb:i=%-5ld\tbm=%-10ld\tbv=%-6d\tbo=%d\tam=%-10ld\tav=%-6d\tao=%d\n" 5   $bid_amnt $bid_volume $bid_ordernum $ask_amnt $ask_volume $ask_ordernum

zb_a_10=`echo $zb_a_10*1000000+0.5 | bc`
zb_a_10=${zb_a_10%.*}

zb_v_10=`echo $zb_v_10*100+0.5 | bc`
zb_v_10=${zb_v_10%.*}

zb_n_10=`echo $zb_n_10+0.5 | bc`
zb_n_10=${zb_n_10%.*}

zs_a_10=`echo $zs_a_10*1000000+0.5 | bc`
zs_a_10=${zs_a_10%.*}

zs_v_10=`echo $zs_v_10*100+0.5 | bc`
zs_v_10=${zs_v_10%.*}

zs_n_10=`echo $zs_n_10+0.5 | bc`
zs_n_10=${zs_n_10%.*}

bid_amnt=$zb_a_10
bid_volume=$zb_v_10
bid_ordernum=$zb_n_10

ask_amnt=$zs_a_10
ask_volume=$zs_v_10
ask_ordernum=$zs_n_10

bid_amnt=${bid_amnt:-0}
bid_volume=${bid_volume:-0}
bid_ordernum=${bid_ordernum:-0}
ask_amnt=${ask_amnt:-0}
ask_volume=${ask_volume:-0}
ask_ordernum=${ask_ordernum:-0}

printf "zb:i=%-5ld\tbm=%-10ld\tbv=%-6d\tbo=%d\tam=%-10ld\tav=%-6d\tao=%d\n" 10   $bid_amnt $bid_volume $bid_ordernum $ask_amnt $ask_volume $ask_ordernum

zb_a_20=`echo $zb_a_20*1000000+0.5 | bc`
zb_a_20=${zb_a_20%.*}

zb_v_20=`echo $zb_v_20*100+0.5 | bc`
zb_v_20=${zb_v_20%.*}

zb_n_20=`echo $zb_n_20+0.5 | bc`
zb_n_20=${zb_n_20%.*}

zs_a_20=`echo $zs_a_20*1000000+0.5 | bc`
zs_a_20=${zs_a_20%.*}

zs_v_20=`echo $zs_v_20*100+0.5 | bc`
zs_v_20=${zs_v_20%.*}

zs_n_20=`echo $zs_n_20+0.5 | bc`
zs_n_20=${zs_n_20%.*}

bid_amnt=$zb_a_20
bid_volume=$zb_v_20
bid_ordernum=$zb_n_20

ask_amnt=$zs_a_20
ask_volume=$zs_v_20
ask_ordernum=$zs_n_20

bid_amnt=${bid_amnt:-0}
bid_volume=${bid_volume:-0}
bid_ordernum=${bid_ordernum:-0}
ask_amnt=${ask_amnt:-0}
ask_volume=${ask_volume:-0}
ask_ordernum=${ask_ordernum:-0}

printf "zb:i=%-5ld\tbm=%-10ld\tbv=%-6d\tbo=%d\tam=%-10ld\tav=%-6d\tao=%d\n" 20   $bid_amnt $bid_volume $bid_ordernum $ask_amnt $ask_volume $ask_ordernum

zb_a_40=`echo $zb_a_40*1000000+0.5 | bc`
zb_a_40=${zb_a_40%.*}

zb_v_40=`echo $zb_v_40*100+0.5 | bc`
zb_v_40=${zb_v_40%.*}

zb_n_40=`echo $zb_n_40+0.5 | bc`
zb_n_40=${zb_n_40%.*}

zs_a_40=`echo $zs_a_40*1000000+0.5 | bc`
zs_a_40=${zs_a_40%.*}

zs_v_40=`echo $zs_v_40*100+0.5 | bc`
zs_v_40=${zs_v_40%.*}

zs_n_40=`echo $zs_n_40+0.5 | bc`
zs_n_40=${zs_n_40%.*}

bid_amnt=$zb_a_40
bid_volume=$zb_v_40
bid_ordernum=$zb_n_40

ask_amnt=$zs_a_40
ask_volume=$zs_v_40
ask_ordernum=$zs_n_40

bid_amnt=${bid_amnt:-0}
bid_volume=${bid_volume:-0}
bid_ordernum=${bid_ordernum:-0}
ask_amnt=${ask_amnt:-0}
ask_volume=${ask_volume:-0}
ask_ordernum=${ask_ordernum:-0}

printf "zb:i=%-5ld\tbm=%-10ld\tbv=%-6d\tbo=%d\tam=%-10ld\tav=%-6d\tao=%d\n" 40   $bid_amnt $bid_volume $bid_ordernum $ask_amnt $ask_volume $ask_ordernum

zb_a_60=`echo $zb_a_60*1000000+0.5 | bc`
zb_a_60=${zb_a_60%.*}

zb_v_60=`echo $zb_v_60*100+0.5 | bc`
zb_v_60=${zb_v_60%.*}

zb_n_60=`echo $zb_n_60+0.5 | bc`
zb_n_60=${zb_n_60%.*}

zs_a_60=`echo $zs_a_60*1000000+0.5 | bc`
zs_a_60=${zs_a_60%.*}

zs_v_60=`echo $zs_v_60*100+0.5 | bc`
zs_v_60=${zs_v_60%.*}

zs_n_60=`echo $zs_n_60+0.5 | bc`
zs_n_60=${zs_n_60%.*}

bid_amnt=$zb_a_60
bid_volume=$zb_v_60
bid_ordernum=$zb_n_60

ask_amnt=$zs_a_60
ask_volume=$zs_v_60
ask_ordernum=$zs_n_60

bid_amnt=${bid_amnt:-0}
bid_volume=${bid_volume:-0}
bid_ordernum=${bid_ordernum:-0}
ask_amnt=${ask_amnt:-0}
ask_volume=${ask_volume:-0}
ask_ordernum=${ask_ordernum:-0}

printf "zb:i=%-5ld\tbm=%-10ld\tbv=%-6d\tbo=%d\tam=%-10ld\tav=%-6d\tao=%d\n" 60   $bid_amnt $bid_volume $bid_ordernum $ask_amnt $ask_volume $ask_ordernum

zb_a_80=`echo $zb_a_80*1000000+0.5 | bc`
zb_a_80=${zb_a_80%.*}

zb_v_80=`echo $zb_v_80*100+0.5 | bc`
zb_v_80=${zb_v_80%.*}

zb_n_80=`echo $zb_n_80+0.5 | bc`
zb_n_80=${zb_n_80%.*}

zs_a_80=`echo $zs_a_80*1000000+0.5 | bc`
zs_a_80=${zs_a_80%.*}

zs_v_80=`echo $zs_v_80*100+0.5 | bc`
zs_v_80=${zs_v_80%.*}

zs_n_80=`echo $zs_n_80+0.5 | bc`
zs_n_80=${zs_n_80%.*}

bid_amnt=$zb_a_80
bid_volume=$zb_v_80
bid_ordernum=$zb_n_80

ask_amnt=$zs_a_80
ask_volume=$zs_v_80
ask_ordernum=$zs_n_80

bid_amnt=${bid_amnt:-0}
bid_volume=${bid_volume:-0}
bid_ordernum=${bid_ordernum:-0}
ask_amnt=${ask_amnt:-0}
ask_volume=${ask_volume:-0}
ask_ordernum=${ask_ordernum:-0}

printf "zb:i=%-5ld\tbm=%-10ld\tbv=%-6d\tbo=%d\tam=%-10ld\tav=%-6d\tao=%d\n" 80   $bid_amnt $bid_volume $bid_ordernum $ask_amnt $ask_volume $ask_ordernum


zb_a_100=`echo $zb_a_100*1000000+0.5 | bc`
zb_a_100=${zb_a_100%.*}

zb_v_100=`echo $zb_v_100*100+0.5 | bc`
zb_v_100=${zb_v_100%.*}

zb_n_100=`echo $zb_n_100+0.5 | bc`
zb_n_100=${zb_n_100%.*}

zs_a_100=`echo $zs_a_100*1000000+0.5 | bc`
zs_a_100=${zs_a_100%.*}

zs_v_100=`echo $zs_v_100*100+0.5 | bc`
zs_v_100=${zs_v_100%.*}

zs_n_100=`echo $zs_n_100+0.5 | bc`
zs_n_100=${zs_n_100%.*}

bid_amnt=$zb_a_100
bid_volume=$zb_v_100
bid_ordernum=$zb_n_100

ask_amnt=$zs_a_100
ask_volume=$zs_v_100
ask_ordernum=$zs_n_100

bid_amnt=${bid_amnt:-0}
bid_volume=${bid_volume:-0}
bid_ordernum=${bid_ordernum:-0}
ask_amnt=${ask_amnt:-0}
ask_volume=${ask_volume:-0}
ask_ordernum=${ask_ordernum:-0}

printf "zb:i=%-5ld\tbm=%-10ld\tbv=%-6d\tbo=%d\tam=%-10ld\tav=%-6d\tao=%d\n" 100   $bid_amnt $bid_volume $bid_ordernum $ask_amnt $ask_volume $ask_ordernum


zb_a_200=`echo $zb_a_200*1000000+0.5 | bc`
zb_a_200=${zb_a_200%.*}

zb_v_200=`echo $zb_v_200*100+0.5 | bc`
zb_v_200=${zb_v_200%.*}

zb_n_200=`echo $zb_n_200+0.5 | bc`
zb_n_200=${zb_n_200%.*}

zs_a_200=`echo $zs_a_200*1000000+0.5 | bc`
zs_a_200=${zs_a_200%.*}

zs_v_200=`echo $zs_v_200*100+0.5 | bc`
zs_v_200=${zs_v_200%.*}

zs_n_200=`echo $zs_n_200+0.5 | bc`
zs_n_200=${zs_n_200%.*}

bid_amnt=$zb_a_200
bid_volume=$zb_v_200
bid_ordernum=$zb_n_200
ask_amnt=$zs_a_200
ask_volume=$zs_v_200
ask_ordernum=$zs_n_200

bid_amnt=${bid_amnt:-0}
bid_volume=${bid_volume:-0}
bid_ordernum=${bid_ordernum:-0}
ask_amnt=${ask_amnt:-0}
ask_volume=${ask_volume:-0}
ask_ordernum=${ask_ordernum:-0}
	

printf "zb:i=%-5ld\tbm=%-10ld\tbv=%-6d\tbo=%d\tam=%-10ld\tav=%-6d\tao=%d\n" 200   $bid_amnt $bid_volume $bid_ordernum $ask_amnt $ask_volume $ask_ordernum

zb_a_500=`echo $zb_a_500*1000000+0.5 | bc`
zb_a_500=${zb_a_500%.*}

zb_v_500=`echo $zb_v_500*100+0.5 | bc`
zb_v_500=${zb_v_500%.*}

zb_n_500=`echo $zb_n_500+0.5 | bc`
zb_n_500=${zb_n_500%.*}

zs_a_500=`echo $zs_a_500*1000000+0.5 | bc`
zs_a_500=${zs_a_500%.*}

zs_v_500=`echo $zs_v_500*100+0.5 | bc`
zs_v_500=${zs_v_500%.*}

zs_n_500=`echo $zs_n_500+0.5 | bc`
zs_n_500=${zs_n_500%.*}

bid_amnt=$zb_a_500
bid_volume=$zb_v_500
bid_ordernum=$zb_n_500

ask_amnt=$zs_a_500
ask_volume=$zs_v_500
ask_ordernum=$zs_n_500

bid_amnt=${bid_amnt:-0}
bid_volume=${bid_volume:-0}
bid_ordernum=${bid_ordernum:-0}
ask_amnt=${ask_amnt:-0}
ask_volume=${ask_volume:-0}
ask_ordernum=${ask_ordernum:-0}

printf "zb:i=%-5ld\tbm=%-10ld\tbv=%-6d\tbo=%d\tam=%-10ld\tav=%-6d\tao=%d\n" 500   $bid_amnt $bid_volume $bid_ordernum $ask_amnt $ask_volume $ask_ordernum




wb_a_0=`echo $wb_a_0*1000000+0.5 | bc`
wb_a_0=${wb_a_0%.*}

wb_v_0=`echo $wb_v_0*100+0.5 | bc`
wb_v_0=${wb_v_0%.*}

wb_n_0=`echo $wb_n_0+0.5 | bc`
wb_n_0=${wb_n_0%.*}

ws_a_0=`echo $ws_a_0*1000000+0.5 | bc`
ws_a_0=${ws_a_0%.*}

ws_v_0=`echo $ws_v_0*100+0.5 | bc`
ws_v_0=${ws_v_0%.*}

ws_n_0=`echo $ws_n_0+0.5 | bc`
ws_n_0=${ws_n_0%.*}

bid_amnt=$wb_a_0
bid_volume=$wb_v_0
bid_ordernum=$wb_n_0

ask_amnt=$ws_a_0
ask_volume=$ws_v_0
ask_ordernum=$ws_n_0

bid_amnt=${bid_amnt:-0}
bid_volume=${bid_volume:-0}
bid_ordernum=${bid_ordernum:-0}
ask_amnt=${ask_amnt:-0}
ask_volume=${ask_volume:-0}
ask_ordernum=${ask_ordernum:-0}

printf "zd:i=%-5ld\tbm=%-10ld\tbv=%-6d\tbo=%d\tam=%-10ld\tav=%-6d\tao=%d\n" 0   $bid_amnt $bid_volume $bid_ordernum $ask_amnt $ask_volume $ask_ordernum

wb_a_5=`echo $wb_a_5*1000000+0.5 | bc`
wb_a_5=${wb_a_5%.*}

wb_v_5=`echo $wb_v_5*100+0.5 | bc`
wb_v_5=${wb_v_5%.*}

wb_n_5=`echo $wb_n_5+0.5 | bc`
wb_n_5=${wb_n_5%.*}

ws_a_5=`echo $ws_a_5*1000000+0.5 | bc`
ws_a_5=${ws_a_5%.*}

ws_v_5=`echo $ws_v_5*100+0.5 | bc`
ws_v_5=${ws_v_5%.*}

ws_n_5=`echo $ws_n_5+0.5 | bc`
ws_n_5=${ws_n_5%.*}

bid_amnt=$wb_a_5
bid_volume=$wb_v_5
bid_ordernum=$wb_n_5

ask_amnt=$ws_a_5
ask_volume=$ws_v_5
ask_ordernum=$ws_n_5

bid_amnt=${bid_amnt:-0}
bid_volume=${bid_volume:-0}
bid_ordernum=${bid_ordernum:-0}
ask_amnt=${ask_amnt:-0}
ask_volume=${ask_volume:-0}
ask_ordernum=${ask_ordernum:-0}

printf "zd:i=%-5ld\tbm=%-10ld\tbv=%-6d\tbo=%d\tam=%-10ld\tav=%-6d\tao=%d\n" 5   $bid_amnt $bid_volume $bid_ordernum $ask_amnt $ask_volume $ask_ordernum

wb_a_10=`echo $wb_a_10*1000000+0.5 | bc`
wb_a_10=${wb_a_10%.*}

wb_v_10=`echo $wb_v_10*100+0.5 | bc`
wb_v_10=${wb_v_10%.*}

wb_n_10=`echo $wb_n_10+0.5 | bc`
wb_n_10=${wb_n_10%.*}

ws_a_10=`echo $ws_a_10*1000000+0.5 | bc`
ws_a_10=${ws_a_10%.*}

ws_v_10=`echo $ws_v_10*100+0.5 | bc`
ws_v_10=${ws_v_10%.*}

ws_n_10=`echo $ws_n_10+0.5 | bc`
ws_n_10=${ws_n_10%.*}

bid_amnt=$wb_a_10
bid_volume=$wb_v_10
bid_ordernum=$wb_n_10

ask_amnt=$ws_a_10
ask_volume=$ws_v_10
ask_ordernum=$ws_n_10

bid_amnt=${bid_amnt:-0}
bid_volume=${bid_volume:-0}
bid_ordernum=${bid_ordernum:-0}
ask_amnt=${ask_amnt:-0}
ask_volume=${ask_volume:-0}
ask_ordernum=${ask_ordernum:-0}

printf "zd:i=%-5ld\tbm=%-10ld\tbv=%-6d\tbo=%d\tam=%-10ld\tav=%-6d\tao=%d\n" 10   $bid_amnt $bid_volume $bid_ordernum $ask_amnt $ask_volume $ask_ordernum

wb_a_20=`echo $wb_a_20*1000000+0.5 | bc`
wb_a_20=${wb_a_20%.*}

wb_v_20=`echo $wb_v_20*100+0.5 | bc`
wb_v_20=${wb_v_20%.*}

wb_n_20=`echo $wb_n_20+0.5 | bc`
wb_n_20=${wb_n_20%.*}

ws_a_20=`echo $ws_a_20*1000000+0.5 | bc`
ws_a_20=${ws_a_20%.*}

ws_v_20=`echo $ws_v_20*100+0.5 | bc`
ws_v_20=${ws_v_20%.*}

ws_n_20=`echo $ws_n_20+0.5 | bc`
ws_n_20=${ws_n_20%.*}

bid_amnt=$wb_a_20
bid_volume=$wb_v_20
bid_ordernum=$wb_n_20

ask_amnt=$ws_a_20
ask_volume=$ws_v_20
ask_ordernum=$ws_n_20

bid_amnt=${bid_amnt:-0}
bid_volume=${bid_volume:-0}
bid_ordernum=${bid_ordernum:-0}
ask_amnt=${ask_amnt:-0}
ask_volume=${ask_volume:-0}
ask_ordernum=${ask_ordernum:-0}

printf "zd:i=%-5ld\tbm=%-10ld\tbv=%-6d\tbo=%d\tam=%-10ld\tav=%-6d\tao=%d\n" 20   $bid_amnt $bid_volume $bid_ordernum $ask_amnt $ask_volume $ask_ordernum

wb_a_40=`echo $wb_a_40*1000000+0.5 | bc`
wb_a_40=${wb_a_40%.*}

wb_v_40=`echo $wb_v_40*100+0.5 | bc`
wb_v_40=${wb_v_40%.*}

wb_n_40=`echo $wb_n_40+0.5 | bc`
wb_n_40=${wb_n_40%.*}

ws_a_40=`echo $ws_a_40*1000000+0.5 | bc`
ws_a_40=${ws_a_40%.*}

ws_v_40=`echo $ws_v_40*100+0.5 | bc`
ws_v_40=${ws_v_40%.*}

ws_n_40=`echo $ws_n_40+0.5 | bc`
ws_n_40=${ws_n_40%.*}

bid_amnt=$wb_a_40
bid_volume=$wb_v_40
bid_ordernum=$wb_n_40

ask_amnt=$ws_a_40
ask_volume=$ws_v_40
ask_ordernum=$ws_n_40

bid_amnt=${bid_amnt:-0}
bid_volume=${bid_volume:-0}
bid_ordernum=${bid_ordernum:-0}
ask_amnt=${ask_amnt:-0}
ask_volume=${ask_volume:-0}
ask_ordernum=${ask_ordernum:-0}

printf "zd:i=%-5ld\tbm=%-10ld\tbv=%-6d\tbo=%d\tam=%-10ld\tav=%-6d\tao=%d\n" 40   $bid_amnt $bid_volume $bid_ordernum $ask_amnt $ask_volume $ask_ordernum

wb_a_60=`echo $wb_a_60*1000000+0.5 | bc`
wb_a_60=${wb_a_60%.*}

wb_v_60=`echo $wb_v_60*100+0.5 | bc`
wb_v_60=${wb_v_60%.*}

wb_n_60=`echo $wb_n_60+0.5 | bc`
wb_n_60=${wb_n_60%.*}

ws_a_60=`echo $ws_a_60*1000000+0.5 | bc`
ws_a_60=${ws_a_60%.*}

ws_v_60=`echo $ws_v_60*100+0.5 | bc`
ws_v_60=${ws_v_60%.*}

ws_n_60=`echo $ws_n_60+0.5 | bc`
ws_n_60=${ws_n_60%.*}

bid_amnt=$wb_a_60
bid_volume=$wb_v_60
bid_ordernum=$wb_n_60

ask_amnt=$ws_a_60
ask_volume=$ws_v_60
ask_ordernum=$ws_n_60

bid_amnt=${bid_amnt:-0}
bid_volume=${bid_volume:-0}
bid_ordernum=${bid_ordernum:-0}
ask_amnt=${ask_amnt:-0}
ask_volume=${ask_volume:-0}
ask_ordernum=${ask_ordernum:-0}

printf "zd:i=%-5ld\tbm=%-10ld\tbv=%-6d\tbo=%d\tam=%-10ld\tav=%-6d\tao=%d\n" 60   $bid_amnt $bid_volume $bid_ordernum $ask_amnt $ask_volume $ask_ordernum

wb_a_80=`echo $wb_a_80*1000000+0.5 | bc`
wb_a_80=${wb_a_80%.*}

wb_v_80=`echo $wb_v_80*100+0.5 | bc`
wb_v_80=${wb_v_80%.*}

wb_n_80=`echo $wb_n_80+0.5 | bc`
wb_n_80=${wb_n_80%.*}

ws_a_80=`echo $ws_a_80*1000000+0.5 | bc`
ws_a_80=${ws_a_80%.*}

ws_v_80=`echo $ws_v_80*100+0.5 | bc`
ws_v_80=${ws_v_80%.*}

ws_n_80=`echo $ws_n_80+0.5 | bc`
ws_n_80=${ws_n_80%.*}

bid_amnt=$wb_a_80
bid_volume=$wb_v_80
bid_ordernum=$wb_n_80

ask_amnt=$ws_a_80
ask_volume=$ws_v_80
ask_ordernum=$ws_n_80

bid_amnt=${bid_amnt:-0}
bid_volume=${bid_volume:-0}
bid_ordernum=${bid_ordernum:-0}
ask_amnt=${ask_amnt:-0}
ask_volume=${ask_volume:-0}
ask_ordernum=${ask_ordernum:-0}

printf "zd:i=%-5ld\tbm=%-10ld\tbv=%-6d\tbo=%d\tam=%-10ld\tav=%-6d\tao=%d\n" 80   $bid_amnt $bid_volume $bid_ordernum $ask_amnt $ask_volume $ask_ordernum


wb_a_100=`echo $wb_a_100*1000000+0.5 | bc`
wb_a_100=${wb_a_100%.*}

wb_v_100=`echo $wb_v_100*100+0.5 | bc`
wb_v_100=${wb_v_100%.*}

wb_n_100=`echo $wb_n_100+0.5 | bc`
wb_n_100=${wb_n_100%.*}

ws_a_100=`echo $ws_a_100*1000000+0.5 | bc`
ws_a_100=${ws_a_100%.*}

ws_v_100=`echo $ws_v_100*100+0.5 | bc`
ws_v_100=${ws_v_100%.*}

ws_n_100=`echo $ws_n_100+0.5 | bc`
ws_n_100=${ws_n_100%.*}

bid_amnt=$wb_a_100
bid_volume=$wb_v_100
bid_ordernum=$wb_n_100

ask_amnt=$ws_a_100
ask_volume=$ws_v_100
ask_ordernum=$ws_n_100

bid_amnt=${bid_amnt:-0}
bid_volume=${bid_volume:-0}
bid_ordernum=${bid_ordernum:-0}
ask_amnt=${ask_amnt:-0}
ask_volume=${ask_volume:-0}
ask_ordernum=${ask_ordernum:-0}

printf "zd:i=%-5ld\tbm=%-10ld\tbv=%-6d\tbo=%d\tam=%-10ld\tav=%-6d\tao=%d\n" 100   $bid_amnt $bid_volume $bid_ordernum $ask_amnt $ask_volume $ask_ordernum

wb_a_200=`echo $wb_a_200*1000000+0.5 | bc`
wb_a_200=${wb_a_200%.*}

wb_v_200=`echo $wb_v_200*100+0.5 | bc`
wb_v_200=${wb_v_200%.*}

wb_n_200=`echo $wb_n_200+0.5 | bc`
wb_n_200=${wb_n_200%.*}

ws_a_200=`echo $ws_a_200*1000000+0.5 | bc`
ws_a_200=${ws_a_200%.*}

ws_v_200=`echo $ws_v_200*100+0.5 | bc`
ws_v_200=${ws_v_200%.*}

ws_n_200=`echo $ws_n_200+0.5 | bc`
ws_n_200=${ws_n_200%.*}

bid_amnt=$wb_a_200
bid_volume=$wb_v_200
bid_ordernum=$wb_n_200

ask_amnt=$ws_a_200
ask_volume=$ws_v_200
ask_ordernum=$ws_n_200

bid_amnt=${bid_amnt:-0}
bid_volume=${bid_volume:-0}
bid_ordernum=${bid_ordernum:-0}
ask_amnt=${ask_amnt:-0}
ask_volume=${ask_volume:-0}
ask_ordernum=${ask_ordernum:-0}

printf "zd:i=%-5ld\tbm=%-10ld\tbv=%-6d\tbo=%d\tam=%-10ld\tav=%-6d\tao=%d\n" 200   $bid_amnt $bid_volume $bid_ordernum $ask_amnt $ask_volume $ask_ordernum

wb_a_500=`echo $wb_a_500*1000000+0.5 | bc`
wb_a_500=${wb_a_500%.*}

wb_v_500=`echo $wb_v_500*100+0.5 | bc`
wb_v_500=${wb_v_500%.*}

wb_n_500=`echo $wb_n_500+0.5 | bc`
wb_n_500=${wb_n_500%.*}

ws_a_500=`echo $ws_a_500*1000000+0.5 | bc`
ws_a_500=${ws_a_500%.*}

ws_v_500=`echo $ws_v_500*100+0.5 | bc`
ws_v_500=${ws_v_500%.*}

ws_n_500=`echo $ws_n_500+0.5 | bc`
ws_n_500=${ws_n_500%.*}

bid_amnt=$wb_a_500
bid_volume=$wb_v_500
bid_ordernum=$wb_n_500

ask_amnt=$ws_a_500
ask_volume=$ws_v_500
ask_ordernum=$ws_n_500

bid_amnt=${bid_amnt:-0}
bid_volume=${bid_volume:-0}
bid_ordernum=${bid_ordernum:-0}
ask_amnt=${ask_amnt:-0}
ask_volume=${ask_volume:-0}
ask_ordernum=${ask_ordernum:-0}

printf "zd:i=%-5ld\tbm=%-10ld\tbv=%-6d\tbo=%d\tam=%-10ld\tav=%-6d\tao=%d\n" 500   $bid_amnt $bid_volume $bid_ordernum $ask_amnt $ask_volume $ask_ordernum



exit 0
