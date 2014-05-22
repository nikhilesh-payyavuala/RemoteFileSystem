
struct filenames {
	string name<>;
	struct filenames *next;
};
struct fdata
{
string text<>;
};
program HW2PROG{
	version CLISERV {
	int CREATE(string,int,string)=1;
	int DELETE(string,string)=2;
	int WRITE(string,string,int,string)=3;
	long FIND(string,string)=4;
	filenames LIST(string)=5; 	
	int READ(string,string,int,string)=6;
	fdata DATA(string,string,int,string)=7; 
	}=1;
}=28914676;