#ifndef TOKENS_H
#define TOKENS_H

#include <stddef.h>

// 使用多行字符串字面值封装
const char qkwstokens[] =
    "<blk> 0\n<sos/eos> 1\n<unk> 2\nA 3\nB 4\nS 5\nI 6\nf 7\nù 8\nǔ 9\n"
    "zh 10\ny 11\nī 12\nsh 13\nēng 14\nuè 15\np 16\niàn 17\nK 18\nL 19\n"
    "P 20\nR 21\nT 22\nM 23\nj 24\nW 25\nq 26\ns 27\nān 28\nb 29\n"
    "ā 30\nl 31\níng 32\niǔ 33\nch 34\nǐ 35\nì 36\nèr 37\nw 38\nr 39\n"
    "án 40\nén 41\nó 42\ng 43\nuǎn 44\nc 45\nāng 46\nǎn 47\nx 48\niān 49\n"
    "ōng 50\nàng 51\nòng 52\nd 53\nài 54\niù 55\ning 56\nū 57\nm 58\nàn 59\n"
    "ǎ 60\ní 61\nz 62\nk 63\nǒu 64\nūn 65\nuó 66\nèng 67\nòu 68\nú 69\n"
    "éng 70\nà 71\niào 72\nāo 73\nào 74\nh 75\nuáng 76\nt 77\niáo 78\nè 79\n"
    "iǎn 80\ner 81\nn 82\nèi 83\nǐng 84\nuā 85\nēi 86\nǎo 87\niú 88\nuàng 89\n"
    "uí 90\nǜ 91\nāi 92\nóu 93\nià 94\nǎng 95\nīn 96\nuà 97\nuǐ 98\nái 99\n"
    "ò 100\nē 101\nóng 102\nuàn 103\né 104\nìn 105\nùn 106\nuò 107\nún 108\n"
    "uì 109\nuān 110\nǐn 111\niāo 112\nōu 113\nuán 114\niǎo 115\ná 116\nér 117\n"
    "èn 118\nC 119\nD 120\nE 121\nO 122\nN 123\nZ 124\něi 125\nián 126\nēn 127\n"
    "iā 128\nǚ 129\náng 130\niě 131\nǒng 132\néi 133\niāng 134\nJ 135\nV 136\n"
    "ín 137\nié 138\nīng 139\nō 140\něr 141\nF 142\nué 143\niǎng 144\nuō 145\n"
    "G 146\nǎi 147\něn 148\nH 149\nen 150\nX 151\ni 152\nǒ 153\nuǒ 154\niè 155\n"
    "iū 156\nU 157\niàng 158\nuāng 159\nuá 160\nQ 161\ně 162\ne 163\niǎ 164\n"
    "iáng 165\nY 166\n○ 167\niē 168\nuī 169\něng 170\nuài 171\náo 172\na 173\n"
    "ou 174\nǔn 175\nuái 176\nuāi 177\nióng 178\nei 179\nń 180\niá 181\niōng 182\n"
    "üè 183\nuē 184\nuǎng 185\nuǎ 186\no 187\nuǎi 188\nuě 189\nǘ 190\nuo 191\n"
    "iǒng 192\nang 193\nia 194\nu 195\nüě 196\n#0 197\n#1 198\n#2 199\n#3 200\n"
    "#4 201\n#5 202\n#6 203\n#7 204\n#8 205\n#9 206\n#10 207\n#11 208\n#12 209\n"
    "#13 210\n#14 211\n#15 212\n#16 213\n#17 214\n#18 215\n#19 216\n#20 217\n"
    "#21 218\n#22 219\n#23 220\n#24 221\n#25 222\n#26 223\n#27 224\n#28 225\n"
    "#29 226";

// 计算字符串大小（不包括终止空字符）
const size_t qkwstokens_size = sizeof(qkwstokens) - 1;

#endif // TOKENS_H
