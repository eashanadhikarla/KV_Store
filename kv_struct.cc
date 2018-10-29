#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <functional>
#include <regex>

#include "kv_struct.h"

using namespace std;
//pHash_Header hash_list[HASH_NUM_MAX];

/*initial a hashlist*/
pHash_List hash_list(void)
{
  u32 i;
  pHash_List list_p;
    //pHash_Header phead;

  list_p = (Hash_List *)malloc(sizeof(Hash_List)); 

  for( i = 0;i < HASH_NUM_MAX;i++ )
  {
    list_p->list[i] = (Hash_Header *)malloc(sizeof(Hash_Header)); 
    list_p->list[i]->next = NULL;
  }     

  return list_p;
}

/*string help()
{
  cout << "Try out some features.";
}

/*insert a node by id*/
string put(pHash_List list_p, string data)
{
  Node *ptail,*pre,*p;
  u32 id, position, len_key, len_value;
  string key, value, response;

  // (u32) len_key
  position = data.find("\n");
  len_key = stoi(data.substr(0, position));
  data = data.substr(position+1);
  // (string) key
  position = data.find("\n");
  key = data.substr(0, position);
  data = data.substr(position+1);
  // (u32) len_value
  position = data.find("\n");
  len_value = stoi(data.substr(0, position));
  // (string) value 
  value = data.substr(position+1);

  std::hash<std::string> h;
  size_t n = h(key);
  id = n % HASH_NUM_MAX;

  ptail = (Node *)malloc(sizeof(Node));
  ptail->next = NULL;
  ptail->value = value;
  ptail->id   = id;
  ptail->key   = key;
  ptail->len_key = len_key;
  ptail->len_value = len_value;


  if( NULL == list_p->list[id]->next )
  {
    list_p->list[id]->next = ptail;
    response = "OK";
    return response;
  }
  pre = list_p->list[id]->next;
  while( pre )
  {
    p = pre;
    pre = pre->next;   
  }
  p->next = ptail;
  response = "OK";
  return response; 
}

/*delete a node by id*/
string del(pHash_List list_p,string data)
{
  Node *psea;
  u32 id, position, len_key;
  string key, response;

  // (u32) len_key
  position = data.find("\n");
  len_key = stoi(data.substr(0, position));
  // (string) key
  key = data.substr(position+1);
  
  std::hash<std::string> h;
  size_t n = h(key);
  id = n % HASH_NUM_MAX;

  psea = list_p->list[id]->next; 
  if( NULL == psea )
  {
   response = "ERROR";
   return response;
 } 
 if(key == psea->key )
 {
   list_p->list[id]->next = psea->next;  
   free(psea);
   response = "OK";
   return response;
 }
 if( NULL == psea->next )
 {
   response = "ERROR";
   return response; 
 } 

 while( key != psea->next->key )
 {
   psea = psea->next;
   if( NULL == psea->next )
   {
     response = "ERROR";
     return response;
   }       
 } 
 psea->next = psea->next->next;
 free(psea->next);

 response = "OK";
 return response;
}


string get(pHash_List list_p, string data)
{
  Node *psea;
  u32 id, position, len_key;
  string key, response;

  // (u32) len_key
  position = data.find("\n");
  len_key = stoi(data.substr(0, position));
  // (string) key
  key = data.substr(position+1);

  std::hash<std::string> h;
  size_t n = h(key);
  id = n % HASH_NUM_MAX;

  psea = list_p->list[id]->next; 
  if( NULL == psea )
  {
    response = "ERROR";
    return response;
  }
  
  if(key == psea->key )
  {
    response = "OK\nRESULT LENGTH : " + to_string(psea->len_value) + "\nRESULT : " + psea->value + "\n";
    return response; 
  } 
  if( NULL == psea->next )
  {
    response = "ERROR";
    return response;
  }

  while( key != psea->next->key )
  {
    psea = psea->next;
    if( NULL == psea->next )
    {
     response = "ERROR";
     return response;
   }   
 }

 response = "OK\nRESULT-LENGTH : " + to_string(psea->len_value) + "\nRESULT : " + psea->value + "\n";
 return response;  
}

string where(pHash_List list_p, string data)
{
  u32 i, num_results, position, len_regexp;
  string sub_str, regexp, response;
  pNode p; 

  // (u32) len_key
  position = data.find("\n");
  len_regexp = stoi(data.substr(0, position));
  // (string) key
  regexp = data.substr(position+1);


  num_results = 0;
  response = "";
  for( i = 0;i < HASH_NUM_MAX; i++)
  {
    p = list_p->list[i]->next;

    while( NULL != p )
    {
      if (regex_match(p->key, regex(regexp)))
      {
        num_results++;
        response = response + to_string(p->len_key) + "\n" + p->key + "\n" + to_string(p->len_value) + "\n" + p->value+ "\n";
      }
      p = p->next;
    }        
  }

  response = to_string(num_results) + "\n" + response;
  return response;

}
/*print the whole hash table*/
void table(pHash_List list_p)
{
  u32 i;
  pNode p; 

  cout << "Print the hash table:" << endl; 

  for( i = 0;i < HASH_NUM_MAX;i++)
  {
    p = list_p->list[i]->next;

    while( NULL != p )
    {
      cout << "ID = " << p->id << " Key Length = " << p->len_key << " key = " << p->key << " Value Length = " << p->len_value << " Value = " << p->value << endl;
      p = p->next;
    }        
  }
  cout << endl;
} 

/*free the whole hash table*/
void free_all_hash(pHash_List list_p)
{
  u32 i;
  pNode p,pn; 

  cout << "Free the whole hashtable" << endl; 

  for( i = 0;i < HASH_NUM_MAX;i++)
  {
    p = list_p->list[i]->next;;
    pn = p;
    if(NULL == p)
    {
      continue;
    }   
    while( NULL != pn )
    {
      p = pn;
      pn = p->next;

      cout << "ID = " << p->id << " key = " << p->key << " Value = " << p->value << endl;  
      free(p);              
    }      
    free(list_p->list[i]);  
  }

} 

