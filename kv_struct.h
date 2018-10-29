#ifndef _KV_STRUCT_
#define _KV_STRUCT_
 
#define HASH_NUM_MAX 65536
 
#define u32 unsigned int
 
typedef struct _Node
{
	std::string key;
    u32 id,len_key,len_value;
    std::string value;
    struct _Node *next;        
}Node,*pNode;
 
typedef struct _Hash_Header
{
    struct _Node *next;    
}Hash_Header,*pHash_Header;
 
typedef struct _Hash_List
{
    struct  _Hash_Header* list[HASH_NUM_MAX];           
}Hash_List,*pHash_List;
 
pHash_List hash_list(void);

std::string put(pHash_List list_p,std::string data);
std::string get(pHash_List list_p, std::string data);
std::string del(pHash_List list_p,std::string data);
std::string where(pHash_List list_p, std::string data);
void table(pHash_List list_p);
//std::string help();

void table(pHash_List list_p);
void free_all_hash(pHash_List list_p);

#endif

