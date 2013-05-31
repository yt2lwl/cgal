#include <iostream>
#include <list>
#include <vector>
#include <iterator>
#include <cassert>

#include <CGAL/iterator.h>
#include <CGAL/use.h>

struct A{};
struct B{};

template<class output>
void check_types(output out){
  typedef typename output::Iterator_tuple T1;
  CGAL_USE_TYPE(typename output::Value_type_tuple);
  CGAL_USE_TYPE(typename output::iterator_category);
  CGAL_USE_TYPE(typename output::value_type);
  CGAL_USE_TYPE(typename output::difference_type);
  CGAL_USE_TYPE(typename output::pointer);
  CGAL_USE_TYPE(typename output::reference);
  T1 tmp=out.get_iterator_tuple();
  tmp=tmp;
}
  
template <class T1,class T2>
void complete_test(std::vector<T1> data1,std::list<T2> data2){

  typedef 
  CGAL::Dispatch_output_iterator<
    CGAL::cpp11::tuple<T1,T2 >,CGAL::cpp11::tuple< T1*,std::back_insert_iterator<std::vector<T2> > > 
  > Dispatcher;

  typedef 
  CGAL::Dispatch_or_drop_output_iterator<
    CGAL::cpp11::tuple<T1,T2 >,CGAL::cpp11::tuple< T1*,std::back_insert_iterator<std::vector<T2> > > 
  > Dropper;
  
  assert(data1.size()==4);
  T1 cont_1[4];
  std::vector<T2> cont_2;
  
  Dispatcher disp=CGAL::dispatch_output<T1,T2>( cont_1,std::back_inserter(cont_2) );
  Dropper drop=CGAL::dispatch_or_drop_output<T1,T2>( cont_1,std::back_inserter(cont_2) );
  
  assert( (CGAL::Is_in_tuple<T1,typename Dispatcher::Value_type_tuple >::value) );
  assert( (CGAL::Is_in_tuple<T2,typename Dispatcher::Value_type_tuple >::value) );
  assert( (!CGAL::Is_in_tuple<A,typename Dispatcher::Value_type_tuple >::value) );
  assert( (CGAL::Is_in_tuple<T1,typename Dropper::Value_type_tuple >::value) );
  assert( (CGAL::Is_in_tuple<T2,typename Dropper::Value_type_tuple >::value) );
  assert( (!CGAL::Is_in_tuple<A,typename Dropper::Value_type_tuple >::value) );


  std::copy(data1.begin(),data1.end(),disp);
  std::copy(data2.begin(),data2.end(),disp);
  assert(cont_2.size()==data2.size());
  for (int i=0;i<4;++i) assert(data1[i]==cont_1[i]);
  
  std::copy(data1.begin(),data1.end(),drop);
  std::copy(data2.begin(),data2.end(),drop);
  *drop++=A();
  assert(cont_2.size()==2 * data2.size());
  for (int i=0;i<4;++i) assert(data1[i]==cont_1[i]);
  
  
  check_types(disp);
  check_types(drop);
  
  disp = disp;
  drop = drop;

  std::back_insert_iterator<std::vector<T2> > bck_ins(cont_2);
  
  T1* d;

  CGAL::cpp11::tie(d, bck_ins) = disp;
  CGAL::cpp11::tie(d, bck_ins) = drop;
}


int main(){
  std::list<int> list1;
  std::list<B> list2;
  std::vector<double> vect1;
  std::vector<char> vect2;
  
  list1.push_back(1); list1.push_back(2);
  list2.push_back(B()); list2.push_back(B());
  
  vect1.push_back(0.); vect1.push_back(0.); vect1.push_back(0.); vect1.push_back(0.);
  vect2.push_back('a'); vect2.push_back('b'); vect2.push_back('c'); vect2.push_back('d');
  
  complete_test(vect1,list1);
  complete_test(vect2,list1);
  complete_test(vect2,list2);
  
  return 0;
}
