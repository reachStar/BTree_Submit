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
            std::pair <Key,Value>num[540];
            int start;
            int weizhi2;
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
            for(int i=0;i<536;i++){
                q=(int *)(p+2*sizeof(int)+i*sizeof(int)*2);
                q2=(int *)(p+3*sizeof(int)+i*sizeof(int)*2);
                tmp.num[i]=std::make_pair(*q,*q2);
            }
            q=(int *)(p+2*sizeof(int)+536*sizeof(int)*2);
            tmp.start=*q;
            q=(int *)(p+3*sizeof(int)+536*sizeof(int)*2);
            tmp.weizhi2=*q;
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
            bool Read(elem &tmp,int plac)const {
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
                for (int i = 0; i < 536; i++) {
                    q = (int *) (p + 2 * sizeof(int) + i * sizeof(int) * 2);
                    q2 = (int *) (p + 3 * sizeof(int) + i * sizeof(int) * 2);
                    tmp.num[i] = std::make_pair(*q, *q2);
                }
                q = (int *) (p + 2 * sizeof(int) + 536 * sizeof(int) * 2);
                tmp.start = *q;
                q = (int *) (p + 3 * sizeof(int) + 536 * sizeof(int) * 2);
                tmp.weizhi2 = *q;
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
            std::cout<<"wawa";
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
            file.write((char *)&n,sizeof(int));
            file.flush();
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
        pair<iterator, OperationResult> insert(const Key& key, const Value& value) {

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
            q.kuaipoint=n;
            return q;
        }
        const_iterator cbegin() const {
            char ch[kuai+2];
            file.read(ch,kuai);
            int *n;
            n=(int *)(ch+2*sizeof(int));
            const_iterator q(file);
            q.place=0;
            q.kuaipoint=n;
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
            q.kuaipoint=n;
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
            q.kuaipoint=n;
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
            char ch[kuai+2];
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
            else return tmp.num[w].second;
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
/*int main(){
    std::cout<<"qaqa";
    sjtu::BTree<int,int> in;
    std::cout<<"haha";
    std::cout<<in.size();
    std::cout<<std::endl<<in.empty();
    sjtu::BTree<int,int>in2=in;
    char ch[kuai+2];
    in.file.seekg(0);
    in2.file.seekg(0);
    in.file.read(ch,kuai);
    for(int i=0;i<kuai;i++) {
        std::cout << ch[i];
    }
    /*
    std::cout<<in2.size();}
    std::fstream tmp("f:\\file10.txt",std::ios::in|std::ios::out|std::ios::binary);
    std::fstream tmp2("f:\\file11.txt",std::ios::in|std::ios::out|std::ios::binary);
    if(!tmp2)return 1;
    char ch[kuai+2];
    tmp.seekg(0);
    tmp2.seekp(0);
    tmp.read(ch,kuai);
    std::cout<<"hahahahahahahaha";
    for(int i=0;i<kuai;i++){
        std::cout<<ch[i];
    }
    tmp2.write(ch,kuai);
    tmp.close();
    tmp2.close();
    return 0;
}*/