#include <bits/stdc++.h>
#define rep(i,l,r) for(int i=l;i<=r;i++)
#define per(i,r,l) for(int i=r;i>=l;i--)
using namespace std;

const int DEBUG=0;

void report(string a){
	printf("%s\n",a.c_str());
	assert(0);
}
struct codetext{
	vector<string> text;
}code;
void getstarted(){
	code.text.clear();
	string templine;
	
	bool cmt=false;
	
	while(getline(cin,templine)){
		string curline="";
		
		char bh=0,ch=0;
		bool lcmt=false;
		
		rep(i,1,templine.size()){
			bh=ch;
			ch=templine[i-1];
			
			if(lcmt) break;
			
			if(cmt){
				if(bh=='*'&&ch=='/'){
					cmt=false;
					bh=ch=0;
				}
				else{
					
				}
			}
			else{
				if(bh=='/'&&ch=='*'){
					cmt=true;
					bh=ch=0;
				}
				else if(bh=='/'&&ch=='/'){
					lcmt=true;
					bh=ch=0;
				}
				else{
					if(bh!=0) curline=curline+bh;
				}
			}
		}
		
		if(!lcmt&&!cmt&&ch!=0) curline=curline+ch;
		
		code.text.push_back(curline);
	}
	if(DEBUG) printf("%d lines\n",code.text.size());
}

#define CI const int

struct token{
	int type,x;
	token(){
		type=x=0;
	}
	token(int _type,int _x){
		type=_type;x=_x;
	}
};
bool operator==(token a,token b){
	return a.type==b.type&&a.x==b.x;
}
namespace TOKEN_CONSTANT{
	CI SIGN=1;
	CI KEY=2;
	CI IDENT=3;
	CI INTCONST=4;
	//STR includes KEY,IDENT,INTCONST
	
	CI LSB=1;//left small bracket, following too
	CI RSB=2;
	CI LMB=3;
	CI RMB=4;
	CI LBB=5;
	CI RBB=6;
	CI SEM=7;//;
	CI COM=8;//,
	
	CI PLUS=9;
	CI MINUS=10;
	CI TIMES=11;
	CI DIVIDE=12;
	CI MODULE=13;
	
	CI EQ=14;
	CI LE=15;
	CI GE=16;
	CI LEQ=17;
	CI GEQ=18;
	CI NEQ=19;
	CI LAND=20;
	CI LOR=21;
	CI LNOT=22;
	
	CI EVAL=23;
	
	CI IF=1;
	CI ELSE=2;
	CI WHILE=3;
	CI BREAK=4;
	CI CONTINUE=5;
	CI RETURN=6;
	CI CONST=7;
	CI INT=8;
	CI VOID=9;
}
using namespace TOKEN_CONSTANT;

map<string,token> str2token;
int identcnt;
int sign2idx[255][255]; 
void splitinit(){
	identcnt=0;
	str2token.clear();
	str2token["if"]=token(KEY,IF);
	str2token["else"]=token(KEY,ELSE);
	str2token["while"]=token(KEY,WHILE);
	str2token["break"]=token(KEY,BREAK);
	str2token["continue"]=token(KEY,CONTINUE);
	str2token["return"]=token(KEY,RETURN);
	str2token["const"]=token(KEY,CONST);
	str2token["int"]=token(KEY,INT);
	str2token["void"]=token(KEY,VOID);
	//let KEY exist
	
	memset(sign2idx,0,sizeof(sign2idx));
	sign2idx['('][0]=LSB;
	sign2idx[')'][0]=RSB;
	sign2idx['['][0]=LMB;
	sign2idx[']'][0]=RMB;
	sign2idx['{'][0]=LBB;
	sign2idx['}'][0]=RBB;
	sign2idx[';'][0]=SEM;
	sign2idx[','][0]=COM;
	sign2idx['+'][0]=PLUS;
	sign2idx['-'][0]=MINUS;
	sign2idx['*'][0]=TIMES;
	sign2idx['/'][0]=DIVIDE;
	sign2idx['%'][0]=MODULE;
	sign2idx['=']['=']=EQ;
	sign2idx['<'][0]=LE;
	sign2idx['>'][0]=GE;
	sign2idx['<']['=']=LEQ;
	sign2idx['>']['=']=GEQ;
	sign2idx['!']['=']=NEQ;
	sign2idx['&']['&']=LAND;
	sign2idx['|']['|']=LOR;
	sign2idx['!'][0]=LNOT;
	sign2idx['='][0]=EVAL;
	//let SIGN link to index
	
	sign2idx['&'][0]=999;
	sign2idx['|'][0]=999;
}
token get_str2token(string s){
	token get=str2token[s];
	if(get.type>0) return get;//existed KET or IDENT
	if(s[0]>='0'&&s[0]<='9'){//INTCONST
		get=token(INTCONST,strtol(s.c_str(),NULL,0));
		//use strtol
	}
	else{//unexisted IDENT
		str2token[s]=get=token(IDENT,++identcnt); 
		//let IDENT exist
	}
	return get;
}

bool chstr(char ch){
	return (ch>='0'&&ch<='9')||(ch>='a'&&ch<='z')||(ch>='A'&&ch<='Z');
}

struct codetokens{
	vector<token> tokens;
}code2;

void split(){
	splitinit();
	
	get_str2token("main");
	get_str2token("getint");
	get_str2token("getch");
	get_str2token("getarray");
	get_str2token("putint");
	get_str2token("putch");
	get_str2token("putarray");
	get_str2token("starttime");
	if(get_str2token("stoptime").x!=9) report("lib func xident wrong");
	
	//let libfunc exist
	
	code2.tokens.clear();
	rep(i,1,code.text.size()){
		string templine=code.text[i-1];
		int p=0,l=templine.size();
		while(p<l){
			char ch=templine[p];
			if(chstr(ch)){
				string tempstr="";
				tempstr=tempstr+ch;
				p++;
				while(p<l){
					char ch2=templine[p];
					if(!chstr(ch2)) break;
					tempstr=tempstr+ch2;
					p++;
				}
				code2.tokens.push_back(get_str2token(tempstr));
			}
			else if(sign2idx[ch][0]>0){
				p++;
				char ch2=0;
				if(p<l){
					ch2=templine[p];
					if(sign2idx[ch][ch2]){
						p++;
					}
					else{
						ch2=0;
					}
				}
				code2.tokens.push_back(token(1,sign2idx[ch][ch2]));
			}
			else if(ch=='\r'||ch==' '||ch=='	'){
				p++;
			}
			else report("unknown char");
		}
	}
	if(DEBUG) printf("%d tokens\n",code2.tokens.size());
	//rep(i,1,code2.tokens.size()){
	//	printf("token %d:: %d %d\n",i-1,code2.tokens[i-1].type,code2.tokens[i-1].x);
	//}
}

struct tnode{
	int type;
	vector<int> sons;
	bool stable;
	tnode(){
		type=0;
		sons.clear();
		stable=false;
	}
	tnode(int _type){
		type=_type;
		sons.clear();
		stable=false;
	}
};
namespace TNODE_CONSTANT{
	CI UNIT=1;
	CI DECL=2;
	CI CDECL=3;
	CI BTYPE=4;
	CI CDEF=5;
	CI CIVAL=6;
	CI VDECL=7;
	CI VDEF=8;
	CI IVAL=9;
	CI FDEF=10;
	CI FTYPE=11;
	CI FFPS=12;
	CI FFP=13;
	CI BLK=14;
	CI BITEM=15;
	CI STMT=16;
	CI EXP=17;
	CI COND=18;
	CI LVAL=19;
	CI PEXP=20;
	CI NUM=21;
	CI UEXP=22;
	CI UOP=23;
	CI FRPS=24;
	CI MEXP=25;
	CI AEXP=26;
	CI REXP=27;
	CI EEXP=28;
	CI LAEXP=29;
	CI LOEXP=30;
	CI CEXP=31;
	
	CI IFS=32;
	CI WHILES=33;
	CI BREAKS=34;
	CI CONTS=35;
	CI RETS=36;
	
	CI XIDENT=37;
	CI XINTCONST=38;
	CI XSIGN=39;
}
using namespace TNODE_CONSTANT;

struct codetree{
	vector<tnode> tree;
}code3;
int newnode(int t){
	int p=code3.tree.size();
	code3.tree.push_back(tnode(t));
	return p;
}
void grow(int x);
int plant(int t){
	int np=newnode(t);
	grow(np);
	return np;
}

int c2_p,c2_l;
void astinit(){
	c2_p=0;
	c2_l=code2.tokens.size();
}
#define c2 code2.tokens
#define push(arg) code3.tree[x].sons.push_back(plant(arg))
#define p0(arg) code3.tree[x].sons.push_back(arg)
#define tt c2[c2_p].type
#define tx c2[c2_p].x
inline bool look(int k,token tar){
	return c2_p+k<=c2_l&&c2[c2_p+k]==tar;
}
inline bool lk0(int tp,int tq){
	return look(0,token(tp,tq));
}
void grow(int x){
	if(DEBUG) printf("%d %d %d\n",x,code3.tree[x].type,c2_p);
		
	if(c2_p>=c2_l) report("code lacking");
	
	switch(code3.tree[x].type){
		case UNIT:{
			while(c2_p<c2_l){
				if(look(2,token(SIGN,LSB))){
					push(FDEF);
				}
				else{
					push(DECL);
				}
			}
			break;
		}
		case DECL:{
			if(lk0(KEY,CONST)){
				push(CDECL);
			}
			else{
				push(VDECL);
			}
			break;
		}
		case CDECL:{
			if(!lk0(KEY,CONST)) report("illegal CDECL");
			c2_p++;
			push(BTYPE);
			push(CDEF);
			while(!lk0(SIGN,SEM)){
				if(!lk0(SIGN,COM)) report("illegal CDECL");
				c2_p++;
				push(CDEF);
			}
			c2_p++;
			break;
		}
		case BTYPE:{
			if(!lk0(KEY,INT)) report("illegal BTYPE");
			c2_p++;
			break;
		}
		case CDEF:{
			push(XIDENT);
			while(lk0(SIGN,LMB)){
				c2_p++;
				push(CEXP);
				if(!lk0(SIGN,RMB)) report("illegal CDEF");
				c2_p++;
			}
			if(!lk0(SIGN,EVAL)) report("illegal CDEF");
			c2_p++;
			push(CIVAL);
			break;
		}
		case CIVAL:{
			if(lk0(SIGN,LBB)){
				c2_p++;
				if(lk0(SIGN,RBB)){
					c2_p++;
				}
				else{
					push(CIVAL);
					while(lk0(SIGN,COM)){
						c2_p++;
						push(CIVAL);
					}
					if(!lk0(SIGN,RBB)) report("illegal CIVAL");
					c2_p++;
				}
			}
			else{
				push(CEXP);
			}
			break;
		}
		case VDECL:{
			push(BTYPE);
			push(VDEF);
			while(!lk0(SIGN,SEM)){
				if(!lk0(SIGN,COM)) report("illegal VDECL");
				c2_p++;
				push(VDEF);
			}
			c2_p++;
			break;
		}
		case VDEF:{
			push(XIDENT);
			while(lk0(SIGN,LMB)){
				c2_p++;
				push(CEXP);//array size must be CEXP, both in CDEF and VDEF
				if(!lk0(SIGN,RMB)) report("illegal VDEF");
				c2_p++;
			}
			if(lk0(SIGN,EVAL)){
				c2_p++;
				push(IVAL);
			}
			break;
		}
		case IVAL:{
			if(lk0(SIGN,LBB)){
				c2_p++;
				if(lk0(SIGN,RBB)){
					c2_p++;
				}
				else{
					push(IVAL);
					while(lk0(SIGN,COM)){
						c2_p++;
						push(IVAL);
					}
					if(!lk0(SIGN,RBB)) report("illegal IVAL");
					c2_p++;
				}
			}
			else{
				push(EXP);
			}
			break;
		}
		
		case FDEF:{
			push(FTYPE);
			push(XIDENT);
			if(!lk0(SIGN,LSB)) report("illegal FDEF");
			c2_p++;
			if(lk0(SIGN,RSB)){
				c2_p++;
			}
			else{
				push(FFPS);
				if(!lk0(SIGN,RSB)) report("illegal FDEF");
				c2_p++;
			}
			push(BLK);
			break;
		}
		case FTYPE:{
			if(!(lk0(KEY,INT)||lk0(KEY,VOID))) report("illegal FTYPE");
			c2_p++;
			break;
		}
		case FFPS:{
			push(FFP);
			while(lk0(SIGN,COM)){
				c2_p++;
				push(FFP);
			}
			break;
		}
		case FFP:{
			push(BTYPE);
			push(XIDENT);
			if(lk0(SIGN,LMB)){
				c2_p++;
				p0(-999);
				if(!lk0(SIGN,RMB)) report("illegal FFP");
				c2_p++;
			}
			while(lk0(SIGN,LMB)){
				c2_p++;
				push(CEXP);
				if(!lk0(SIGN,RMB)) report("illegal FFP");
				c2_p++;
			}
			break;
		}
		
		case BLK:{
			if(!lk0(SIGN,LBB)) report("illegal BLK");
			c2_p++;
			while(!lk0(SIGN,RBB)){
				push(BITEM);
			}
			c2_p++;
			break;
		}
		case BITEM:{
			if(lk0(KEY,CONST)||lk0(KEY,INT)){
				push(DECL);
			}
			else{
				push(STMT); 
			}
			break;
		}
		case STMT:{
			if(lk0(SIGN,SEM)){
				// do nothing
			}
			else if(lk0(SIGN,LBB)){
				push(BLK);
			}
			else if(lk0(KEY,IF)){
				push(IFS);
			}
			else if(lk0(KEY,WHILE)){
				push(WHILES);
			}
			else if(lk0(KEY,BREAK)){
				push(BREAKS);
			}
			else if(lk0(KEY,CONTINUE)){
				push(CONTS);
			}
			else if(lk0(KEY,RETURN)){
				push(RETS);
			}
			else{
				int testp=c2_p;
				while(1){
					if(c2[testp].type==SIGN&&c2[testp].x==SEM) break;
					if(c2[testp].type==SIGN&&c2[testp].x==EVAL) break;
					testp++;
					if(testp>=c2_l) report("code lacking");
				}
				
				if(c2[testp].x==SEM){
					push(EXP);
					if(!lk0(SIGN,SEM)) report("illegal STMT");
					c2_p++;
				}
				else if(c2[testp].x==EVAL){
					push(LVAL);
					if(!lk0(SIGN,EVAL)) report("illegal STMT");
					c2_p++;
					push(EXP);
					if(!lk0(SIGN,SEM)) report("illegal STMT");
					c2_p++;
				}
				else report("nt");
			}
			break;
		}
		case IFS:{
			if(!lk0(KEY,IF)) report("illegal IFS");
			c2_p++;
			if(!lk0(SIGN,LSB)) report("illegal IFS");
			c2_p++;
			push(COND);
			if(!lk0(SIGN,RSB)) report("illegal IFS");
			c2_p++;
			push(STMT);
			if(lk0(KEY,ELSE)){
				c2_p++;
				push(STMT);
			}
			break;
		}
		case WHILES:{
			if(!lk0(KEY,WHILE)) report("illegal WHILES");
			c2_p++;
			if(!lk0(SIGN,LSB)) report("illegal WHILES");
			c2_p++;
			push(COND);
			if(!lk0(SIGN,RSB)) report("illegal WHILES");
			c2_p++;
			push(STMT);
			break;
		}
		case BREAKS:{
			if(!lk0(KEY,BREAK)) report("illegal BREAKS");
			c2_p++;
			if(!lk0(SIGN,SEM)) report("illegal BREAKS");
			c2_p++;
			break;
		}
		case CONTS:{
			if(!lk0(KEY,CONTINUE)) report("illegal CONTS");
			c2_p++;
			if(!lk0(SIGN,SEM)) report("illegal CONTS");
			c2_p++;
			break;
		}
		case RETS:{
			if(!lk0(KEY,RETURN)) report("illegal RETS");
			c2_p++;
			push(EXP);
			if(!lk0(SIGN,SEM)) report("illegal RETS");
			c2_p++;
			break;
		}
		
		case EXP:{
			push(AEXP);
			break;
		}
		case COND:{
			push(LOEXP);
			break;
		}
		case LVAL:{
			push(XIDENT);
			while(lk0(SIGN,LMB)){
				c2_p++;
				push(EXP);
				if(!lk0(SIGN,RMB)) report("illegal VDEF");
				c2_p++;
			}
			break;
		}
		case PEXP:{
			if(lk0(SIGN,LSB)){
				c2_p++;
				push(EXP);
				if(!lk0(SIGN,RSB)) report("illegal PEXP");
				c2_p++;
			}
			else if(tt==INTCONST){
				push(NUM);
			}
			else if(tt==IDENT){
				push(LVAL);
			}
			else report("illegal PEXP");
			break;
		}
		case NUM:{
			push(XINTCONST);
			break;
		}
		case UEXP:{
			if(tt==IDENT&&look(1,token(SIGN,LSB))){
				push(XIDENT);
				if(!lk0(SIGN,LSB)) report("illegal UEXP");
				c2_p++;
				if(lk0(SIGN,RSB)){
					c2_p++;
				}
				else{
					push(FRPS);
					if(!lk0(SIGN,RSB)) report("illegal UEXP");
					c2_p++;
				}
			}
			else if(lk0(SIGN,PLUS)||lk0(SIGN,MINUS)||lk0(SIGN,LNOT)){
				push(UOP);
				push(UEXP);
			}
			else{
				push(PEXP);
			}
			break;
		}
		case UOP:{
			push(XSIGN);
			break;
		}
		case FRPS:{
			push(EXP);
			while(lk0(SIGN,COM)){
				c2_p++;
				push(EXP);
			}
			break;
		}
		case MEXP:{
			push(UEXP);
			while(lk0(SIGN,TIMES)||lk0(SIGN,DIVIDE)||lk0(SIGN,MODULE)){
				push(XSIGN);
				push(UEXP);
			}
			break;
		}
		case AEXP:{
			push(MEXP);
			while(lk0(SIGN,PLUS)||lk0(SIGN,MINUS)){
				push(XSIGN);
				push(MEXP);
			}
			break;
		}
		case REXP:{
			push(AEXP);
			while(lk0(SIGN,LE)||lk0(SIGN,GE)||lk0(SIGN,LEQ)||lk0(SIGN,GEQ)){
				push(XSIGN);
				push(AEXP);
			}
			//make no sense: a>=a>=a>=a>=a?
			break;
		}
		case EEXP:{
			push(REXP);
			while(lk0(SIGN,EQ)||lk0(SIGN,NEQ)){
				push(XSIGN);
				push(REXP);
			}
			//make no sense: f==f==f==f==f?
			break;
		}
		case LAEXP:{
			push(EEXP);
			while(lk0(SIGN,LAND)){
				push(XSIGN);
				push(EEXP);
			}
			break;
		}
		case LOEXP:{
			push(LAEXP);
			while(lk0(SIGN,LOR)){
				push(XSIGN);
				push(LAEXP);
			}
			break;
		}
		case CEXP:{
			push(AEXP);
			break;
		}

		case XIDENT:
		case XINTCONST:
		case XSIGN:{
			p0(tx);
			c2_p++;
			break;
		}
		
	}
	
	if(DEBUG) printf("%d %d %d ok\n",x,code3.tree[x].type,c2_p);
}
#undef c2
#undef push
#undef p0
#undef tt
#undef tx

void ast(){
	astinit();
	
	code3.tree.clear();
	if(plant(UNIT)!=0) report("rooted wrongly");
	
	if(DEBUG) printf("%d tnodes\n",code3.tree.size());
}

int low;//low < SDiff meaning in stack

namespace varmanage{
	vector<int> pool;
	vector<int> permitted;
	
	CI SDiff=1e9;
	
	struct varcontent{
		int x;//point into pool
		int d;
		vector<int> m;
		varcontent(){//null varcontent init
			x=-1;d=0;m.clear();
		}
		varcontent(int v){//short for varcontent(0,vector<int>())
			if(v!=0) report("misuse varcontent(int)");
			
			x=pool.size();d=0;m.clear();
			pool.push_back(0);
			permitted.push_back(0);
			
			x=x-low+SDiff;
		}
		varcontent(int _d,vector<int> _m){
			if(_m.size()!=_d) report("wrong dimenson when construct");
			
			x=pool.size();d=_d;m=_m;
			//vector::= is ok
			int n=1;
			rep(i,1,d) n*=m[i-1];
			rep(i,1,n){
				pool.push_back(0);
				permitted.push_back(0);
			}
			
			x=x-low+SDiff;
		}
		varcontent(int _x,int _d,vector<int> _m){
			x=_x;d=_d;m=_m;
		}
		varcontent vop(int k){
			if(d==0) report("can only op with pointer");
			
			int n=1;
			rep(i,2,d) n*=m[i-1];
			
			vector<int> tempvec;
			tempvec.clear();
			rep(i,2,d) tempvec.push_back(m[i-1]);
			
			return varcontent(x+k*n,d-1,tempvec);
		}
	};
	
	int depth;
	varcontent vstd0;
	vector<varcontent> vstd;
	vector<vector<varcontent> > varh;//varh[dep] for depth==dep
	void varhisinit(){
		vstd.clear();
		rep(i,1,identcnt){//determined from split.h
			vstd.push_back(vstd0);
		}
		depth=0;
	}
	void varhn(){
		varh.push_back(vstd);
		depth++;
	}
	void varhm(){
		varh.pop_back();
		depth--;
	}
	void varhis(int id,varcontent a){
		varh[depth-1][id-1]=a;
	}
	varcontent varget(int id){
		int d=depth-1;
		while(d>=0&&varh[d][id-1].x==-1) d--;
		if(d<0) report("cannot find the varname(var id)");
		
		return varh[d][id-1];
	}
	
}
using namespace varmanage;

namespace constantexp{
	int uop(int op,int a){
		switch(op){
			case PLUS:{
				return +a;
				break;
			}
			case MINUS:{
				return -a;
				break;
			}
			case LNOT:{
				return !a;
				break;
			}
		}
	}
	int bop(int op,int a,int b){
		switch(op){
			case PLUS:{
				return a+b;
				break;
			}
			case MINUS:{
				return a-b;
				break;
			}
			case TIMES:{
				return a*b;
				break;
			}
			case DIVIDE:{
				return a/b;
				break;
			}
			case MODULE:{
				return a%b;
				break;
			}
			case EQ:{
				return a==b;
				break;
			}
			case LE:{
				return a<b;
				break;
			}
			case GE:{
				return a>b;
				break;
			}
			case LEQ:{
				return a<=b;
				break;
			}
			case GEQ:{
				return a>=b; 
				break;
			}
			case NEQ:{
				return a!=b;
				break;
			}
			case LAND:{
				return a&&b;
				break;
			}
			case LOR:{
				return a||b;
				break;
			}
		}
	}
	
	#define c3 code3.tree
	#define sz c3[x].sons.size()
	#define son(arg) c3[x].sons[arg]
	#define sot(arg) c3[son(arg)].type
	#define sub(arg) cexp(son(arg))
	#define pass res=sub(0)
	#define p0 res=son(0)
	int cexp(int x){//in CEXP, only result value matters
		int res,temp;
		switch(c3[x].type){
			case EXP:
			case COND:{
				pass;
				break;
			}
			case LVAL:{
				varcontent va=varget(sub(0));
				
				if(va.x>=SDiff){//in stack, but CEXP ignore the difference
					va.x-=SDiff;
				}
				
				rep(i,2,sz){
					va=va.vop(sub(i-1));
				}
				if(va.d>0) report("LVAL content must be single int in CEXP");
				if(va.x<0||va.x>=pool.size()) report("unknown(dangerous) memory used in CEXP");
				if(permitted[va.x]==0) report("unconst memory used in CEXP");
				return pool[va.x];
				
				break;
			}
			case PEXP:{
				pass;//same for EXP,LVAL,NUM
				break;
			}
			case NUM:{
				pass;
				break;
			}
			case UEXP:{
				if(sot(0)==PEXP){
					pass;
				}
				else if(sot(0)==UOP){
					temp=sub(1);
					res=uop(sub(0),temp);
				}
				else if(sot(0)==XIDENT){//ident(FRPS)
					report("nmsl(func in CUEXP)");
				}
				else report("unexpected (C)UEXP");
				break;
			}
			case UOP:{
				pass;
				break;
			}
			
			case MEXP:
			case AEXP:
			case REXP:
			case EEXP:
			case LAEXP:
			case LOEXP:{
				if(sz%2==0) report("even elements in some type LONG (C)EXP");
				
				temp=sub(0);
				res=temp;
				rep(i,1,sz/2){
					temp=sub(i*2);
					res=bop(sub(i*2-1),res,temp);
				}
				break;
			}
			
			case CEXP:{//entry is here
				pass;
				break;
			}
			
			case XIDENT:
			case XINTCONST:
			case XSIGN:{
				p0;
				break;
			}
		}
		return res;
	}
	#undef c3
	#undef sz
	#undef son
	#undef sot
	#undef sub
	#undef pass
	#undef p0
}
using namespace constantexp;

struct stmt{
	int op;
	int u,v;
	int env;
	
	stmt(){
		op=u=v=env=0;
	}
	stmt(int _op,int _u,int _v,int _env){
		op=_op;u=_u;v=_v;env=_env;
	}
};

namespace STMT_CONSTANT{
	CI LD=1;//load mem[u] reg_v
	CI ST=2;//store reg_u mem[v] 
	CI OP=3;//reg_1 = [reg_1] op(u) reg_2
	CI EV=4;//reg_u = v
	CI LB=5;//.l_u:
	CI JM=6;//if (u) jump to l_v
	
	CI DF=7;//func_u[v][space[funh[u-1]]]:
	CI EF=8;//end func_u
	CI RE=9;//return
	
	CI LDX=10;//load mem[reg_1] reg_1
	CI STX=11;//store reg_2 mem[reg_1] 
	CI CL=12;//call func_u
	CI SP=13;//calculate position for stk[u] as reg_v
								//mem[low+u]
}
using namespace STMT_CONSTANT;

struct eblock{
	vector<stmt> ss;
	int x;
	eblock(){
		ss.clear();
		x=0;
	}
};

int lbcnt;

int lfcnt;
vector<int> space;
vector<int> fnx;

vector<int> funh;

int loopcnt;
vector<int> lbc,lbb;

void _init(){
	varhisinit();
	
	lbcnt=0;
	
	lfcnt=0;
	space.clear();
	fnx.clear();
	
	funh.clear();
	rep(i,1,identcnt) funh.push_back(0);
	
	loopcnt=0;
	lbc.clear();
	lbb.clear();
	
	low=1e9;
}

#define c3 code3.tree
#define sz c3[x].sons.size()
#define son(arg) c3[x].sons[arg]
#define sot(arg) c3[son(arg)].type
#define sub(arg) build(son(arg))
#define pass cur=sub(0)
#define p0 cur.x=son(0)
#define sbp rep(arg,1,temp.ss.size()) cur.ss.push_back(temp.ss[arg-1])
#define ssp(arg,brg,crg) cur.ss.push_back(stmt(arg,brg,crg,low))

eblock build(int x);

eblock align(varcontent a,int x){
	if(c3[x].type!=IVAL&&c3[x].type!=CIVAL) report("not IVAL nor CIVAL to align");
	
	eblock cur,temp;
	
	int p=a.x;
	
	if(sz==1&&sot(0)==CEXP){
		if(a.d>0) report("cannot align single (c)int to (c)array");
		temp=build(son(0));//.x: constant
		pool[p]=temp.x;
		permitted[p]=1;
		ssp(EV,1,temp.x);
		ssp(ST,1,p);
	}
	else if(sz==1&&sot(0)==EXP){
		if(a.d>0) report("cannot align single int to array");
		temp=build(son(0));//.x: address
		sbp;
		ssp(LD,temp.x,1);
		ssp(ST,1,p);
	}
	else{
		int n=1;
		rep(i,2,a.m.size()){
			n*=a.m[i-1];
		}
		
		rep(i,1,sz){
			int y=son(i-1);//type: (C)IVAL
			if(c3[y].sons.size()==1&&(c3[c3[y].sons[0]].type==CEXP||c3[c3[y].sons[0]].type==EXP)){
				temp=align(varcontent(p,0,vector<int>()),y);
				sbp;
				p++;
			}
			else{
				while((p-a.x)%n!=0){//align
					if(c3[x].type==CIVAL){
						pool[p]=0;
						permitted[p]=1;
					}
					ssp(EV,1,0);
					ssp(ST,1,p);
					p++;
				}
				temp=align(a.vop((p-a.x)/n),y);
				sbp;
				p+=n;
			}
		}
		
		n=n*a.m[0];
		if(p-a.x>n) report("too much to align");
		while(p-a.x<n){//align
			if(c3[x].type==CIVAL){
				pool[p]=0;
				permitted[p]=1;
			}
			ssp(EV,1,0);
			ssp(ST,1,p);
			p++;
		}
	}
	
	return cur;
}

eblock build(int x){
	if(DEBUG) printf("building %d(%d)\n",x,c3[x].type);
	
	eblock cur,temp;
	switch(c3[x].type){
		case UNIT:{
			varhn();
			
			rep(i,1,sz){
				temp=sub(i-1);
				sbp;
			}
			
			varhm();
			break;
		}
		case DECL:{
			pass;
			break;
		}
		case CDECL:{
			rep(i,2,sz){
				temp=sub(i-1);
				sbp;
			}
			break;
		}
		case BTYPE:{
			report("NAN");
			break;
		}
		case CDEF:{
			int id=sub(0).x;
			vector<int> m;
			m.clear();
			rep(i,2,sz-1){
				m.push_back(sub(i-1).x);
			}
			
			varcontent nv(m.size(),m);
			varhis(id,nv);
			
			temp=align(nv,son(sz-1));
			sbp;
			
			break;
		}
		case CIVAL:{
			report("NAN");
			break;
		}
		case VDECL:{
			rep(i,2,sz){
				temp=sub(i-1);
				sbp;
			}
			break;
		}
		case VDEF:{
			int id=sub(0).x;
			vector<int> m;
			m.clear();
			
			if(sot(sz-1)==IVAL){
				rep(i,2,sz-1){
					m.push_back(sub(i-1).x);
				}
			}
			else{
				rep(i,2,sz){
					m.push_back(sub(i-1).x);
				}
			}
			
			varcontent nv(m.size(),m);
			varhis(id,nv);
			
			if(sot(sz-1)==IVAL){
				temp=align(nv,son(sz-1));
				sbp;
			}
			
			break;
		}
		case FDEF:{
			varhn();
			
			int id=sub(1).x;
			funh[id-1]=lfcnt++;
			low=pool.size();
			
			int k;//#FFP
			varcontent a[8];
			
			if(sot(2)==FFPS){
				int y=son(2);//type: FFPS
				k=c3[y].sons.size();
				
				ssp(DF,id,k);//def func
				
				rep(i,1,k){
					int z=c3[y].sons[i-1];//type: FFP
					
					int zx=c3[z].sons[1];//type: xident
					int id=build(zx).x;
					
					int zsz=c3[z].sons.size();
					if(zsz>=3){//be pointer FFP
						if(c3[z].sons[2]!=-999) report("where is my -999?");
						
						vector<int> zm;
						zm.clear();
						zm.push_back(-999);
						rep(j,4,zsz){
							int zz=c3[z].sons[j-1];//type: CEXP
							zm.push_back(build(zz).x);
						}
						
						a[i-1]=varcontent(0);
						ssp(ST,9+i,a[i-1].x);//arg part
						
						a[i-1].x=-a[i-1].x-200;//untrivial meaning
						a[i-1].d=zsz-2;
						a[i-1].m=zm;
					}
					else{
						a[i-1]=varcontent(0);
						ssp(ST,9+i,a[i-1].x);//arg part
					}
					
					varhis(id,a[i-1]);
				}
			}
			else{
				ssp(DF,id,0);//def func
				
				k=0;//no arg part
			}
			
			temp=sub(sz-1);//block part
			sbp;
			
			ssp(RE,0,0);//must return when natural end
			ssp(EF,id,0);//end func
			
			varhm();
			
			space.push_back(pool.size()-low);
			low=1e9;
			break;
		}
		case FTYPE:{
			report("NAN");
			break;
		}
		case FFPS:{
			report("NAN");
			break;
		}
		case FFP:{
			report("NAN");
			break;
		}
		case BLK:{
			varhn();
			
			rep(i,1,sz){
				temp=sub(i-1);
				sbp;
			}
			
			varhm();
			break;
		}
		case BITEM:{
			pass;
			break;
		}
		case STMT:{
			if(sz==0){//;
				
			}
			else if(sz>=2){
				temp=sub(0);
				sbp;
				int p=temp.x;
				
				temp=sub(1);
				sbp;
				ssp(LD,temp.x,2);
				
				ssp(LD,p,1);
				ssp(STX,0,0);
			}
			else{//same for EXP,IFS,WHILES,BREAKS,CONTS,RETS
				pass;
			}
			break;
		}
		case IFS:{
			int lb1=lbcnt++;
			int lb2=lbcnt++;
			
			temp=sub(0);
			sbp;
			ssp(LD,temp.x,1);
			ssp(JM,1,lb1);
			
			if(sz>=3){//have else part
				temp=sub(2);
				sbp;
			}
			
			ssp(JM,0,lb2);
			ssp(LB,lb1,0);
			
			temp=sub(1);
			sbp;
			
			ssp(LB,lb2,0);
			break;
		}
		case WHILES:{
			int lb1=lbcnt++;
			int lb2=lbcnt++;
			int lb3=lbcnt++;
			
			lbc.push_back(lb3);
			lbb.push_back(lb2);
			loopcnt++;
			
			ssp(LB,lb3,0);
			
			temp=sub(0);
			sbp;
			ssp(LD,temp.x,1);
			ssp(JM,1,lb1);
			
			ssp(JM,0,lb2);
			ssp(LB,lb1,0);
			
			temp=sub(1);
			sbp;
			
			ssp(JM,0,lb3);
			ssp(LB,lb2,0);
			
			lbc.pop_back();
			lbb.pop_back();
			loopcnt--;
			break;
		}
		case BREAKS:{
			ssp(JM,0,lbb[loopcnt-1]);
			break;
		}
		case CONTS:{
			ssp(JM,0,lbc[loopcnt-1]);
			break;
		}
		case RETS:{
			if(sz>=1){
				temp=sub(0);
				sbp;
				ssp(LD,temp.x,9+1);
			}
			
			ssp(RE,0,0);
			break;
		}
		
		case EXP:
		case COND:{
			pass;
			break;
		}
		case LVAL:{//specially: .x := the address of target address
								//not the address of result value
			int id=sub(0).x;
			varcontent a=varget(id);
			
			if(a.x<0){
				if(a.x>-200) report("unexpected negative varcontent.x");
				
				ssp(LD,-a.x-200,1);//untrivial understanding(of meaning)
			}
			else{
				if(a.x>=SDiff){//in stack
					ssp(SP,a.x-SDiff,1);
				}
				else{
					ssp(EV,1,a.x);
				}
			}
			
			rep(i,2,sz){
				if(i>=3){
					ssp(EV,2,a.m[i-2]);
					ssp(OP,TIMES,0);
				}
				temp=sub(i-1);
				sbp;
				ssp(LD,2,temp.x);
				ssp(OP,PLUS,0);
			}
			
			cur.x=varcontent(0).x;
			ssp(ST,1,cur.x);
			
			if(sz-1<a.d){//LVAL be not single int in EXP(because from FRPS)
				cur.x=-cur.x-400;
			}
			break;
		}
		case PEXP:{
			if(sot(0)==EXP){
				pass;
			}
			else if(sot(0)==NUM){
				ssp(EV,1,sub(0).x);
				
				cur.x=varcontent(0).x;
				ssp(ST,1,cur.x);
				
			}
			else if(sot(0)==LVAL){
				temp=sub(0);
				sbp;
				
				if(temp.x<0){
					if(temp.x>-400) report("unexpected negative eblock.x");
					
					cur.x=-temp.x-400;
				}
				else{
					ssp(LD,temp.x,1);
					ssp(LDX,0,0);
				
					cur.x=varcontent(0).x;
					ssp(ST,1,cur.x);
				}
			}
			else report("uexpected PEXP");
			break;
		}
		case NUM:{
			pass;//actually only .x, also := result value
			break;
		}
		case UEXP:{
			if(sot(0)==PEXP){
				pass;
			}
			else if(sot(0)==UOP){
				temp=sub(1);
				sbp;
				ssp(LD,temp.x,1);
				
				ssp(OP+444,sub(0).x,0);//tag uop
				
				cur.x=varcontent(0).x;
				ssp(ST,1,cur.x);
			}
			else if(sot(0)==XIDENT){//XIDENT(FRPS)
				int id=sub(0).x;
				//int lf=funh[id-1];
				
				if(sz>=2){//have params part
					int y=son(1);//type: FRPS
					rep(i,1,c3[y].sons.size()){
						int z=c3[y].sons[i-1];//type EXP'
						
						temp=build(z);
						sbp;
						
						ssp(LD,temp.x,9+i);
					}
				}
				
				ssp(CL,id,0);
				
				cur.x=varcontent(0).x;
				ssp(ST,9+1,cur.x);
			}
			else report("unexpected UEXP");
			break;
		}
		case UOP:{
			pass;//actually only .x, also := result value
			break;
		}
		case FRPS:{
			report("NAN");
			break;
		}
		
		//following same content for different type LONG EXP
		case MEXP:
		case AEXP:
		case REXP:
		case EEXP:
		case LAEXP:
		case LOEXP:{
			if(sz%2==0) report("even elements in some type LONG EXP");
			
			if(sz==1){
				pass;//optimize
			}
			else{
				temp=sub(0);
				sbp;
				ssp(LD,temp.x,1);
				
				rep(i,1,sz/2){
					temp=sub(i*2);
					sbp;
					ssp(LD,temp.x,2);
					
					ssp(OP,sub(i*2-1).x,0);
				}
				
				cur.x=varcontent(0).x;
				ssp(ST,1,cur.x);
			}
			
			break;
		}
		
		case CEXP:{//speically: .x := result value
							//not the address of result value
			cur.x=cexp(x);
			break;
		}
		
		case XIDENT:
		case XINTCONST:
		case XSIGN:{//similar with CEXP
			p0;
			break;
		}
	}
	if(DEBUG) printf("builded %d[%d]\n",x,cur.ss.size());
	return cur;
}
#undef c3
#undef sz
#undef son
#undef sot
#undef sub
#undef pass
#undef p0
#undef sbp
#undef ssp

eblock code4;
void gen(){
	_init();
	code4=build(0);
	if(DEBUG) printf("%d statements\n",code4.ss.size());
}

void work(int x){
	switch(x){
		case PLUS:{
			printf("add reg1, reg1, reg2\n");
			break;
		}
		case MINUS:{
			printf("sub reg1, reg1, reg2\n");
			break;
		}
		case TIMES:{
			printf("mul reg1, reg1, reg2\n");
			break;
		}
		case DIVIDE:{
			printf("div reg1, reg1, reg2\n");
			break;
		}
		case MODULE:{
			printf("rem reg1, reg1, reg2\n");
			break;
		}
		case LE:{
			printf("slt reg1, reg1, reg2\n");
			break;
		}
		case GE:{
			printf("sgt reg1, reg1, reg2\n");
			break;
		}
		case LEQ:{
			printf("sgt reg1, reg1, reg2\n");
			printf("seqz reg1, reg1\n");
			break;
		}
		case GEQ:{
			printf("slt reg1, reg1, reg2\n");
			printf("seqz reg1, reg1\n");
			break;
		}
		case LAND:{
			printf("snez reg1, reg1\n");
			printf("snez s0, reg2\n");
			printf("and reg1, reg1, t0\n");
			break;
		}
		case LOR:{
			printf("or reg1, reg1, reg2\n");
			printf("snez reg1, reg1\n");
			break;
		}
		case NEQ:{
			printf("xor reg1, reg1, reg2\n");
			printf("snez reg1, reg1\n");
			break;
		}
		case EQ:{
			printf("xor reg1, reg1, reg2\n");
			printf("seqz reg1, reg1\n");
			break;
		}
		case PLUS+444:{
			//do nothing
			break;
		}
		case MINUS+444:{
			printf("neg reg1, reg1\n");
			break;
		}
		case LNOT+444:{
			printf("seqz reg1, reg1\n");
			break;
		}
	}
}

void fname(int x){
	switch(x){
		case 1:{
			printf("main");
			break;
		}
		case 2:{
			printf("getint");
			break;
		}
		case 3:{
			printf("getch");
			break;
		}
		case 4:{
			printf("getarray");
			break;
		}
		case 5:{
			printf("putint");
			break;
		}
		case 6:{
			printf("putch");
			break;
		}
		case 7:{
			printf("putarray");
			break;
		}
		case 8:{
			printf("_sys_starttime");
			break;
		}
		case 9:{
			printf("_sys_stoptime");
			break;
		}
		default:{
			printf("func%d",x);
			break;
		}
	}
}

int STK;

void output(stmt a){
	int u=a.u,v=a.v;
	switch(a.op){
		case LD:{
			if(u>=SDiff){
				printf("  li s0, %d\n",(u-SDiff)*4);
				if(v>=10){
					printf("  lw a%d, s0(sp)\n",v-10);
				}
				else{
					printf("  lw s%d, s0(sp)\n",v);
				}
			}
			else{
				printf("  li s0, %d\n",u*4);
				if(v>=10){
					printf("  lw a%d, s0(s3)\n",v-10);
				}
				else{
					printf("  lw s%d, s0(s3)\n",v);
				}
			}
			break;
		}
		case ST:{
			if(v>=SDiff){
				printf("  li s0, %d\n",(v-SDiff)*4);
				if(u>=10){
					printf("  sw a%d, s0(sp)\n",u-10);
				}
				else{
					printf("  sw s%d, s0(sp)\n",u);
				}
			}
			else{
				printf("  li s0, %d\n",v*4);
				if(u>=10){
					printf("  sw a%d, s0(s3)\n",u-10);
				}
				else{
					printf("  sw s%d, s0(s3)\n",u);
				}
			}
			break;
		}
		case OP:{
			work(u);
			break;
		}
		case EV:{
			if(u>=10){
				printf("  li a%d, %d\n",u-10,v);
			}
			else{
				printf("  li s%d, %d\n",u,v);
			}
			break;
		}
		case LB:{
			printf(".l%d:\n",u);
			break;
		}
		case JM:{
			if(u==0){
				printf("j .l%d\n",v);
			}
			else{
				printf("li s0, 0\n");
				if(u>=10){
					printf("bne a%d, s0, .l%d\n",u-10,v);
				}
				else{
					printf("bne s%d, s0, .l%d\n",u,v);
				}
			}
			break;
		}
		case DF:{
			STK=(space[funh[u-1]]/4+1)*16;
			
			printf("  .text\n");
			printf("  .align 2\n");
			printf("  .global ");fname(u);printf("\n");
			printf("  .type ");fname(u);printf(", @function\n");
			
			fname(u);printf(":\n");
			printf("  addi sp, sp, %d\n",-STK);//..
			printf("  sw ra, %d(sp)\n",STK-4);
			break;
		}
		case EF:{
			printf("  .size ");fname(u);printf(", .-");fname(u);printf("\n");
			break;
		}
		case RE:{
			printf("  lw ra, %d(sp)\n",STK-4);
			printf("  addi sp, sp, %d\n",STK);//..
			printf("  ret\n");
			break;
		}
		case LDX:{
			printf("  lw reg1, reg1(s3)\n");
			break;
		}
		case STX:{
			printf("  rw reg1, reg2(s3)\n");
			break;
		}
		case CL:{
			printf("  call ");fname(u);printf("\n");
			break;
		}
		case SP:{
			printf("  li s0, %d\n",u*4);
			if(v>=10){
				printf("  add a%d, sp, s0\n",v-10);
			}
			else{
				printf("  add s%d, sp, s0\n",v);
			}
			break;
		}
	}
}
void endup(){
	printf("  .comm v0, %d, 4\n",pool.size()*4);
	printf("  la s3, v0\n");
	
	rep(i,1,code4.ss.size()){
		output(code4.ss[i-1]);
	}
}

int main(int argc,char* argv[]){
	freopen(argv[2],"r",stdin);
	freopen(argv[4],"w",stdout);
	
	getstarted();
	split();
	
	ast();
	
	gen();
	endup();
	
	fclose(stdin);
	fclose(stdout);
	
	return 0;
}
