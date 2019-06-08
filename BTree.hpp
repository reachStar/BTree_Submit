//
// Created by 郑文鑫 on 2019-03-09.
//

#include "utility.hpp"
#include <functional>
#include <cstddef>
#include "exception.hpp"
#include <map>
#include<fstream>
#include<iostream>
#define kuai 4096
namespace sjtu {
    template <class Key, class Value, class Compare = std::less<Key> >
    class BTree {
    public:
        mutable std::fstream file;
        struct elem{
            int flagAndParent;
            int here;
            int oneSonPoint;
            std::pair <Key,Value>num[508];
            int start;
            int weizhi2;
            int min;
            elem(){start=0;}
        };
    public:
        void Write(elem &tmp,int place){
            file.seekp(place);
            file.write((char*)&tmp,kuai);
            file.flush();
        }
        bool Read(elem &tmp,int place)const{
            file.seekg(place);
            int *q;
            char p[kuai+2];
            file.read(p,kuai);
            q=(int *)p;
            tmp.flagAndParent=*q;
            q=(int *)(p+sizeof(int));
            tmp.here=*q;
            q=(int *)(p+2*sizeof(int));
            tmp.oneSonPoint=*q;
            int *q2;
            for(int i=0;i<508;i++){
                q=(int *)(p+3*sizeof(int)+i*sizeof(int)*2);
                q2=(int *)(p+4*sizeof(int)+i*sizeof(int)*2);
                tmp.num[i]=std::make_pair(*q,*q2);
            }
            q=(int *)(p+3*sizeof(int)+508*sizeof(int)*2);
            tmp.start=*q;
            q=(int *)(p+4*sizeof(int)+508*sizeof(int)*2);
            tmp.weizhi2=*q;
            q=(int *)(p+5*sizeof(int)+508*sizeof(int)*2);
            tmp.min=*q;
            if(tmp.flagAndParent<0)return 1;
            else return 0;
        }
        // Your private members go here
    public:
        typedef pair<const Key, Value> value_type;


        class const_iterator;
        class iterator {
        public:
            int kuaipoint;
            int place;
            std::fstream fileIte;
            // Your private members go here
        public:
            void Write(elem &tmp,int plac){
                fileIte.seekp(plac);
                fileIte.write((char*)&tmp,kuai);
                fileIte.flush();
            }
            bool Read(elem &tmp,int plac)/*const*/ {
                fileIte.seekg(plac);
                int *q;
                char p[kuai + 2];
                fileIte.read(p, kuai);
                q = (int *) p;
                tmp.flagAndParent = *q;
                q = (int *) (p + sizeof(int));
                tmp.here = *q;
                q = (int *) (p + 2 * sizeof(int));
                tmp.oneSonPoint = *q;
                int *q2;
                for (int i = 0; i < 508; i++) {
                    q = (int *) (p + 2 * sizeof(int) + i * sizeof(int) * 2);
                    q2 = (int *) (p + 3 * sizeof(int) + i * sizeof(int) * 2);
                    tmp.num[i] = std::make_pair(*q, *q2);
                }
                q = (int *) (p + 2 * sizeof(int) + 508 * sizeof(int) * 2);
                tmp.start = *q;
                q = (int *) (p + 3 * sizeof(int) + 508 * sizeof(int) * 2);
                tmp.weizhi2 = *q;
                q=(int *)(p+4*sizeof(int)+508*sizeof(int)*2);
                tmp.min=*q;
                if (tmp.flagAndParent < 0)return 1;
                else return 0;
            }
            bool modify(const Value& value){
                elem tmp;
                Read(tmp,kuaipoint);
                if(tmp.flagAndParent>0)return 0;
                tmp.num[place].second=value;
                Write(tmp,kuaipoint);
                return 1;
            }
            iterator() {

                // TODO Default Constructor
            }
            iterator(const iterator& other):fileIte(other.fileIte){
                kuaipoint=other.kuaipoint;
                place=other.place;
                // TODO Copy Constructor
            }
            iterator(std::fstream w):fileIte(w){

            }
            // Return a new iterator which points to the n-next elements
            iterator operator++(int) {
                iterator q=*this;
                elem tmp;
                Read(tmp,kuaipoint);
                if(tmp.start>=place){
                    place++;
                    return q;
                }
                kuaipoint=tmp.flagAndParent*(-1);
                place=0;
                return q;
                // Todo iterator++
            }
            iterator& operator++() {
                elem tmp;
                Read(tmp,kuaipoint);
                if(tmp.start>=place){
                    place++;
                    return *this;
                }
                kuaipoint=tmp.flagAndParent*(-1);
                place=0;
                return *this;
                // Todo ++iterator
            }
            iterator operator--(int) {
                iterator q=*this;
                elem tmp;
                Read(tmp,kuaipoint);
                if(place==0) {
                    kuaipoint = tmp.weizhi2;
                    Read(tmp, kuaipoint);
                    place=tmp.start-1;
                    return q;
                }
                place--;
                return q;
                // Todo iterator--
            }
            iterator& operator--() {
                elem tmp;
                Read(tmp,kuaipoint);
                if(place==0) {
                    kuaipoint = tmp.weizhi2;
                    Read(tmp, kuaipoint);
                    place=tmp.start-1;
                    return *this;
                }
                place--;
                return *this;
                // Todo --iterator
            }
            // Overloaded of operator '==' and '!='
            // Check whether the iterators are same
            bool operator==(const iterator& rhs) const {
                if(fileIte==rhs.fileIte&&kuaipoint==rhs.kuaipoint&&place==rhs.place)return 1;
                else return 0;
                // Todo operator ==
            }
            bool operator==(const const_iterator& rhs) const {
                if(fileIte==rhs.fileIte&&kuaipoint==rhs.kuaipoint&&place==rhs.place)return 1;
                else return 0;
                // Todo operator ==
            }
            bool operator!=(const iterator& rhs) const {
                if(!(*this==rhs))return 1;
                else return 0;
                // Todo operator !=
            }
            bool operator!=(const const_iterator& rhs) const {
                if(!(*this==rhs))return 1;
                else return 0;
                // Todo operator !=
            }
        };
        class const_iterator {
            int kuaipoint;
            int place;
            std::fstream &fileIte;
            // it should has similar member method as iterator.
            //  and it should be able to construct from an iterator.
        private:
            // Your private members go here
        public:
            const_iterator() {
                // TODO
            }
            const_iterator(std::fstream w):fileIte(w){

            }
            const_iterator(const const_iterator& other):fileIte(other.fileIte) {
                kuaipoint=other.kuaipoint;
                place=other.place;
                // TODO
            }
            const_iterator(const iterator& other):fileIte(other.fileIte){
                kuaipoint=other.kuaipoint;
                place=other.place;
                // TODO
            }
            // And other methods in iterator, please fill by yourself.
        };
        // Default Constructor and Copy Constructor
        BTree() {
            file.open("f:\\file10.txt",std::ios::in|std::ios::out|std::ios::binary);
            int n=0;
            file.seekp(0);
            file.write((char *)&n,sizeof(int));
            file.flush();
            n=kuai;
            file.write((char *)&n,sizeof(int));
            file.flush();
            n=kuai*2;
            file.write((char *)&n,sizeof(int));
            file.flush();
            n=kuai*2;
            file.write((char *)&n,sizeof(int));
            file.flush();
            elem tmp;
            tmp.start=-1;
            tmp.flagAndParent=0;
            tmp.here=kuai;
            Write(tmp,kuai);
            // Todo Default
        }
        BTree(const BTree& other) {
            file.open("f:\\file11.txt",std::ios::in|std::ios::out|std::ios::binary);
            char tmp[kuai+2];
            other.file.seekg(0);
            other.file.read(tmp,kuai);
            int *q;
            q=(int *)(tmp+sizeof(int)*3);
            file.seekp(0);
            file.write(tmp,kuai);
            file.flush();
            for(int i=kuai;i<=*q;i+=kuai){
                elem t;
                other.Read(t,i);
                Write(t,i);
            }
            // Todo Copy
        }
        BTree& operator=(const BTree& other) {
            if(other==*this)return *this;
            char tmp[kuai+2];
            other.file.seekg(0);
            other.file.read(tmp,kuai);
            int *q;
            q=(int *)(tmp+sizeof(int)*3);
            file.seekp(0);
            file.write(tmp,kuai);
            file.flush();
            for(int i=kuai;i<=*q;i+=kuai){
                elem t;
                other.Read(t,i);
                Write(t,i);
            }
            // Todo Assignment
        }
        ~BTree() {
            file.close();
            // Todo Destructor
        }
        // Insert: Insert certain Key-Value into the database
        // Return a pair, the first of the pair is the iterator point to the new
        // element, the second of the pair is Success if it is successfully inserted
        //pair<iterator, OperationResult> insert(const Key& key, const Value& value) {
        void insert(const Key& key, const Value& value) {
            /*char ch[kuai+2];
            file.seekg(0);
            file.read(ch,kuai);
            int *q=(int *)(ch+sizeof(int));
            int dizhi=*q;
            elem tmp;
            int m;
            while(1){
                Read(tmp,dizhi);
                if(tmp.flagAndParent<0)break;
                if(tmp.start==-1){m=-1;break;}
                if(tmp.flagAndParent>=0) {
                    if(tmp.start==0){
                        dizhi=tmp.oneSonPoint;m=-1;continue;
                    }
                    for (int i = 0; i < tmp.start; i++) {
                        if (key >= tmp.num[i].first)continue;
                        else dizhi = tmp.num[i]
                    }
                    if(i==0){m=-1;dizhi=tmp.oneSonPoint;}
                    else {m=i-1;dizhi=tmp.num[i-1].second;}
                }
            }
            if(tmp.flagAndParent>=0){
                tmp.start++;
                q=(int *)(ch+sizeof(int)*3);
                dizhi=*q;
                tmp.oneSonPoint=dizhi;
                elem w;
                w.flagAndParent=-1*tmp.here;
                w.star=1;
                w.weizhi2=0;
                w.here=dizhi;
                w.num[0]=std::make_pair(key,value);
                Write(tmp,tmp.here);
                Write(w,w.here);
                file.seekp(0);
                q=(int *)ch;
                dizhi=*q+1;
                file.seekp(0);
                file.write(&dizhi,sizeof(int));
                file.flush();
                return pair;
            }
            else {
                int i=0;
                for(;i<tmp.start;i++) {
                    if(key>=tmp.num[i].first)continue;
                    else break;
                }

                for(int j=tmp.start++;j>i;j--) {
                    tmp.num[j]=tmp.num[j-1];
                }
                tmp.num[i]=std::make_pair(key,value);
                q=(int *)ch;
                dizhi=*q+1;
                file.seekp(0);
                file.write(&dizhi,sizeof(int));
                file.flush();
                if(tmp.start==508){
                    std::pair ch2[255];
                    for(int i=0;i<254;i++){
                        ch2[i]=tmp.num[i+254];
                    }
                    tmp.start=254;
                    Write(tmp,tmp.here);
                    q=(int *)(ch+sizeof(int)*3);
                    dizhi=*q;
                    int moweizhi=dizhi+kuai;
                    file.seekp(3*sizeof(int));
                    file.write((char *)&moweizhi,sizeof(int));
                    file.flush();
                    elem z;
                    z.here=duzhi;
                    z.start=254;
                    for(int i=0;i<254;i++){
                        z.num[i]=ch2[i];
                    }
                    z.flagAndParent=tmp.flagAndParent;
                    Write(z,z.here);
                    int x=1000;
                    tmp.flagAndParent*=-1;
                    std::pair k(z.num[0].first,z.here);
                    while(x>=508){
                        Read(tmp,tmp.flagAndParent);
                        for(int i=tmp.start++;i>m+1;i++){
                            tmp.num[i]=tmp.num[i-1];
                        }
                        tmp.num[m+1]=k;
                        x=tmp.flagAndParent;
                    }
                }
            }*/
            char ch[kuai+2];
            file.seekg(0);
            file.read(ch,kuai);
            //int *q=(int *)ch;
            //int w=*q+1;
            //file.seekp(0);
            //file.write((char *)&w,sizeof(int));
            int *tmp;
            int dizhi;
            tmp=(int *)(ch+sizeof(int));
            charu(*tmp,key,value);

        }
        void charu(int n,Key key,Value value){
            elem tmp;
            Read(tmp,n);
            int min;
            //std::cout<<tmp.flagAndParent<<' '<<tmp.here<<' '<<tmp.start<<std::endl;
            if(tmp.flagAndParent>0){
                int i=0;
                for(;i<tmp.start;i++){
                    if(key>=tmp.num[i].first)continue;
                    else break;
                }
                if(i==0) {
                    if (key < tmp.min)tmp.min = key;
                    charu(tmp.oneSonPoint, key, value);
                }
                else
                    charu(tmp.num[i-1].second,key,value);
                Read(tmp,n);
                if(tmp.start>=508)erfen(tmp);
            }
            else if(tmp.flagAndParent==0){
                if(tmp.start==-1){
                    tmp.min=key;
                    char ch[kuai+2];
                    file.seekg(0);
                    file.read(ch,kuai);
                    int *tmp2;
                    int dizhi;
                    tmp2=(int *)(ch+sizeof(int)*3);
                    dizhi=*tmp2;
                    tmp.start=1;
                    elem tmp3;
                    tmp3.start=1;
                    tmp3.flagAndParent=-1*tmp.here;
                    tmp3.here=dizhi;
                    tmp3.min=key;
                    tmp3.num[0]=std::make_pair(key,value);
                    tmp.oneSonPoint=tmp3.here;
                    Write(tmp3,tmp3.here);
                    Write(tmp,tmp.here);
                    dizhi+=kuai;
                    file.seekp(sizeof(int)*3);
                    file.write((char *)&dizhi,sizeof(int));
                    return ;
                }
                else{
                    int i=0;
                    for(;i<tmp.start;i++){
                        if(key>=tmp.num[i].first)continue;
                        else break;
                    }
                    if(i==0) {
                        if (key < tmp.min)tmp.min = key;
                        charu(tmp.oneSonPoint, key, value);
                    }
                    else
                        charu(tmp.num[i-1].second,key,value);
                    Read(tmp,n);
                    if(tmp.start>=508)erfen(tmp);
                }
            }
            else{
                if(tmp.start==-1)tmp.min=key;
                int i=0;
                for(;i<tmp.start;i++){
                    if(key>=tmp.num[i].first)continue;
                    else break;
                }
                for(int j=tmp.start++;j>i;j--){
                    tmp.num[j]=tmp.num[j-1];
                }
                tmp.num[i]=std::make_pair(key,value);
                Write(tmp,n);
                if(tmp.start>=508)erfen(tmp);
            }
        }
        void erfen(elem &tmp){
            if(tmp.flagAndParent<0){
                elem t2;
                char ch[kuai+2];
                file.seekg(0);
                file.read(ch,kuai);
                int *tmp2;
                int dizhi;
                tmp2=(int *)(ch+sizeof(int)*3);
                dizhi=*tmp2;
                t2.here=dizhi;
                dizhi+=kuai;
                file.seekp(sizeof(int)*3);
                file.write((char *)&dizhi,sizeof(int));
                t2.flagAndParent=tmp.flagAndParent;
                t2.start=254;
                for(int i=0;i<254;i++){
                    t2.num[i]=tmp.num[i+254];
                }
                t2.min=t2.num[0].first;
                Write(t2,t2.here);
                tmp.start=254;
                Write(tmp,tmp.here);
                elem t3;
                Read(t3,tmp.flagAndParent*(-1));
                int j=0;
                if(t3.oneSonPoint==tmp.here){
                    for(int i=t3.start;i>0;i--){
                        t3.num[i]=t3.num[i-1];
                    }
                    t3.num[0]=std::make_pair(t2.min,t2.here);
                    t3.start++;
                    Write(t3,t3.here);
                    return ;
                }
                else {
                    for(;j<t3.start;j++){
                        if(t3.num[j].second==tmp.here)break;
                    }
                    for(int i=t3.start;i>j+1;i--){
                        t3.num[i]=t3.num[i-1];
                    }
                    t3.num[j+1]=std::make_pair(t2.min,t2.here);
                    t3.start++;
                    Write(t3,t3.here);
                    return ;
                }
            }
            else if(tmp.flagAndParent>0){
                elem t2;
                char ch[kuai+2];
                file.seekg(0);
                file.read(ch,kuai);
                int *tmp2;
                int dizhi;
                tmp2=(int *)(ch+sizeof(int)*3);
                dizhi=*tmp2;
                t2.here=dizhi;
                dizhi+=kuai;
                file.seekp(sizeof(int)*3);
                file.write((char *)&dizhi,sizeof(int));
                t2.flagAndParent=tmp.flagAndParent;
                t2.start=254;
                int i;
                for(i=0;i<253;i++){
                    t2.num[i]=tmp.num[i+255];
                }
                t2.oneSonPoint=tmp.num[i+254].second;
                elem qaq;
                Read(qaq,t2.oneSonPoint);
                t2.min=qaq.min;
                Write(t2,t2.here);
                tmp.start=254;
                Write(tmp,tmp.here);
                elem t3;
                Read(t3,tmp.flagAndParent*(-1));
                int j=0;
                if(t3.oneSonPoint==tmp.here){
                    for(int i=t3.start;i>0;i--){
                        t3.num[i]=t3.num[i-1];
                    }
                    t3.num[0]=std::make_pair(t2.min,t2.here);
                    t3.start++;
                    Write(t3,t3.here);
                    return ;
                }
                else {
                    for(;j<t3.start;j++){
                        if(t3.num[j].second==tmp.here)break;
                    }
                    for(int i=t3.start;i>j+1;i--){
                        t3.num[i]=t3.num[i-1];
                    }
                    t3.num[j+1]=std::make_pair(t2.min,t2.here);
                    t3.start++;
                    Write(t3,t3.here);
                    return ;
                }
            }
            else {
                elem t2;
                char ch[kuai+2];
                file.seekg(0);
                file.read(ch,kuai);
                int *tmp2;
                int dizhi;
                tmp2=(int *)(ch+sizeof(int)*3);
                dizhi=*tmp2;
                elem t3;
                t3.here=dizhi;
                dizhi+=kuai;
                t2.flagAndParent=t3.here;
                file.seekp(sizeof(int)*2);
                file.write((char*)&t3.here,sizeof(int));
                t2.here=dizhi;
                dizhi+=kuai;
                file.seekp(sizeof(int)*3);
                file.write((char *)&dizhi,sizeof(int));
                t2.flagAndParent=tmp.flagAndParent;
                t2.start=254;
                int i;
                for(i=0;i<253;i++){
                    t2.num[i]=tmp.num[i+255];
                }
                t2.oneSonPoint=tmp.num[i+254].second;
                elem qaq;
                Read(qaq,t2.oneSonPoint);
                t2.min=qaq.min;
                Write(t2,t2.here);
                tmp.start=254;
                Write(tmp,tmp.here);
                t3.flagAndParent=0;
                t3.oneSonPoint=tmp.here;
                t3.start=1;
                t3.num[0]=std::make_pair(t2.min,t2.here);
                Write(t3,t3.here);
            }
        }
        // Erase: Erase the Key-Value
        // Return Success if it is successfully erased
        // Return Fail if the key doesn't exist in the database
        OperationResult erase(const Key& key) {
            // TODO erase function
            return Fail;  // If you can't finish erase part, just remaining here.
        }
        // Return a iterator to the beginning
        iterator begin() {
            char ch[kuai+2];
            file.read(ch,kuai);
            int *n;
            n=(int *)(ch+2*sizeof(int));
            iterator q(file);
            q.place=0;
            q.kuaipoint=*n;
            return q;
        }
        const_iterator cbegin() const {
            char ch[kuai+2];
            file.read(ch,kuai);
            int *n;
            n=(int *)(ch+2*sizeof(int));
            const_iterator q(file);
            q.place=0;
            q.kuaipoint=*n;
            return q;
        }
        // Return a iterator to the end(the next element after the last)
        iterator end() {
            elem tmp;
            char ch[kuai+2];
            file.read(ch,kuai);
            int *n;
            n=(int *)(ch+3*sizeof(int));
            iterator q(file);
            q.kuaipoint=*n;
            Read(tmp,n);
            q.place=tmp.start;
            return q;
        }
        const_iterator cend() const {
            elem tmp;
            char ch[kuai+2];
            file.read(ch,kuai);
            int *n;
            n=(int *)(ch+3*sizeof(int));
            const_iterator q(file);
            q.kuaipoint=*n;
            Read(tmp,n);
            q.place=tmp.start;
            return q;
        }
        // Check whether this BTree is empty
        bool empty() const {
            char tmp[kuai+2];
            file.seekg(0);
            file.read(tmp,kuai);
            int *q;
            q=(int *)tmp;
            if(*q)return 1;
            else return 0;
        }
        // Return the number of <K,V> pairs
        size_t size() const {
            char tmp[kuai+2];
            file.seekg(0);
            file.read(tmp,kuai);
            int *q;
            q=(int *)tmp;
            return *q;
        }
        // Clear the BTree
        void clear() {
            int n=0;
            file.seekp(0);
            file.write((char *)&n,sizeof(int));
            file.flush();
            n=kuai;
            file.write((char *)&n,sizeof(int));
            file.flush();
            n=kuai*2;
            file.write((char *)&n,sizeof(int));
            file.flush();
            file.write((char *)&n,sizeof(int));
            file.flush();
        }
        // Return the value refer to the Key(key)
        Value at(const Key& key){
            /*char ch[kuai+2];
            file.seekg(0);
            file.read(ch,kuai);
            int *point=(int *)(ch+sizeof(int));
            int flag=1;
            elem tmp;
            while(flag>0){
                Read(tmp,point);
                flag=tmp.flagAndParent;
                if(flag>0){
                    int w=erfenchazhao(tmp,0,tmp.start-1);
                    point=tmp.num[w].second;
                }
            }
            int w=erfenchazhao(tmp,0,tmp.start-1);
            if(tmp.num[w].first!=key)return 0;
            else return tmp.num[w].second;*/
            char ch[kuai+2];
            file.seekg(0);
            file.read(ch,kuai);
            int *point=(int*)(ch+sizeof(int));
            int dizhi=*point;
            elem tmp;
            while(1){
                Read(tmp,dizhi);
                int flag=tmp.flagAndParent;
                if(flag>=0){
                    int i=0;
                    for(;i<tmp.start;i++){
                        if(key>=tmp.num[i].first)continue;
                        else break; ;
                    }
                    if(i==0)dizhi=tmp.oneSonPoint;
                    else dizhi=tmp.num[i-1].second;
                    Read(tmp,dizhi);
                }
                else break;
            }
            for(int i=0;i<tmp.start;i++){
                if(tmp.num[i].first==key)return tmp.num[i].second;
            }
            return 0;
        }
        /**
         * Returns the number of elements with key
         *   that compares equivalent to the specified argument,
         * The default method of check the equivalence is !(a < b || b > a)
         */
        size_t count(const Key& key) const {}
        /**
         * Finds an element with key equivalent to key.
         * key value of the element to search for.
         * Iterator to an element with key equivalent to key.
         *   If no such element is found, past-the-end (see end()) iterator is
         * returned.
         */
        iterator find(const Key& key) {}
        const_iterator find(const Key& key) const {}
        int erfenchazhao(elem tmp,int start,int stop){


        }
    };
}  // namespace sjtu
