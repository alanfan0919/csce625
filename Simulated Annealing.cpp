#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <algorithm>
#include <string>
#include <vector>

using namespace std;
class City{
public:
	City(int _x,int _y,string str):x(_x),y(_y),name(str){}
	double distanceTo(City* c2){
		double xDiff=abs(x-c2->x),yDiff=abs(y-c2->y);
		return sqrt(xDiff*xDiff+yDiff*yDiff);
	}
	int getX(){
		return x;
	}
	int getY(){
		return y;
	}
	void printCity(){
		cout<<name<<"("<<x<<","<<y<<")";
	}
private:
	int x,y;
	string name;
};

class Cities{
private:
	static vector<City*> cityset;
public:
	static void addCity(City* newcity){
		cityset.push_back(newcity);
	}
	static City* getCity(int index){
		return cityset[index];
	}
	static int getSize(){
		return cityset.size();
	}
};
vector<City* > Cities::cityset;

class Tour{
private:
	vector<City*> tour;
public:
	Tour(){}
	Tour(Tour* oldtour){ //construct a tour which is a random neighbor of old tour
		for(int i=0;i<oldtour->getTourSize();i++){
			tour.push_back(oldtour->getCity(i));
		}
		//srand(unsigned(time(NULL)));
		int p1=0,p2=0;
		while(p1==0)
			p1=rand()%oldtour->getTourSize();
		while(p2==0||p1==p2)
			p2=rand()%oldtour->getTourSize();
		swap(tour[p1],tour[p2]);
	}
	vector<City*> getTour(){
		return tour;
	}
	void randomGenerate(){
		for(int i=0;i<Cities::getSize();i++)
			tour.push_back(Cities::getCity(i));
		random_shuffle(tour.begin()+1,tour.end());
	}

	double getDis(){
		double totalDis=0;
		for(int i=0;i<tour.size();i++){
			totalDis+=tour[i]->distanceTo(tour[(i+1)%tour.size()]);
		}
		return totalDis;
	}
	
	int getTourSize(){
		return tour.size();
	}
	City* getCity(int index){
		return tour[index];
	}
};
class SimulatedAnnealing{
private: Tour* bestTour;
public:
	SimulatedAnnealing():bestTour(NULL){}
	void simulatedAnnealingSearch(){
		if(Cities::getSize()==0){
			City* urunmchi=new City(68,54,"URunmchi"); City* lhasa=new City(71,131,"Lhasa"); City* xining=new City(122,98,"Xining");
			City* chengdu=new City(131,130,"Chengdu"); City* kunming=new City(124,161,"Kunming"); City* yinchuan =new City(141,89,"Yinchuan");
			City* chongqing=new City(142,136,"Chongqing"); City* guiyang=new City(144,152,"Guiyang"); City* nanning=new City(152,172,"Nanning");
			City* xian=new City(153,110,"Xian"); City* hohhot=new City(163,75,"Hohhot");City* taiyuan=new City(168,91,"Taiyuan"); 
			City* zhengzhou=new City(175,108,"Zhengzhou"); City* wuhan=new City(180,129,"Wuhan"); City* changsha=new City(174,142,"Changsha");
			City* guangzhou=new City(177,169,"Guangzhou"); City* hongkong=new City(182,174,"HongKong"); City* haikou=new City(163,188,"Haikou");
			City* beijing=new City(184,76,"Beijing"); City* tianjing=new City(188,82,"Tianjing"); City* shijiazhuang=new City(177,88,"Shijiazhuang");
			City* jinan=new City(188,97,"Jinan"); City* hefei=new City(192,120,"Hefei"); City* nanchang=new City(187,138,"Nanchang");
			City* fuzhou=new City(205,150,"Fuzhou"); City* taipei=new City(217,156,"Taipei"); City* hangzhou=new City(206,127,"Hangzhou");
			City* shanghai=new City(212,122,"Shanghai"); City* nanjing=new City(201,120,"Nanjing"); City* shenyang=new City(209,64,"Shenyang");
			City* changchun=new City(214,51,"Changchun"); City* harbin=new City(217,41,"Harbin");

			Cities::addCity(hongkong); Cities::addCity(lhasa); Cities::addCity(xining); Cities::addCity(chengdu);
			Cities::addCity(kunming); Cities::addCity(yinchuan); Cities::addCity(chongqing); Cities::addCity(guiyang);
			Cities::addCity(nanning); Cities::addCity(xian); Cities::addCity(hohhot); Cities::addCity(taiyuan);
			Cities::addCity(zhengzhou); Cities::addCity(wuhan); Cities::addCity(changsha); Cities::addCity(guangzhou);
			Cities::addCity(urunmchi); Cities::addCity(haikou); Cities::addCity(beijing); Cities::addCity(tianjing);
			Cities::addCity(shijiazhuang); Cities::addCity(jinan); Cities::addCity(hefei); Cities::addCity(nanchang);
			Cities::addCity(fuzhou); Cities::addCity(taipei); Cities::addCity(hangzhou); Cities::addCity(shanghai);
			Cities::addCity(nanjing); Cities::addCity(shenyang); Cities::addCity(changchun); Cities::addCity(harbin);
		}
		
		double tempe=1765;
		double coolingrate=5.395e-6;

		Tour* currentTour;
		if(bestTour==NULL){
			currentTour=new Tour();
			currentTour->randomGenerate();
		}
		else
			currentTour=bestTour;
		
		cout<<"Initial tour distance is "<<currentTour->getDis()<<endl;//*2006.82/156.6
		bestTour=currentTour;
		int bestIte=0;
		int ite=0;
		while(tempe>8){
			ite++;
			Tour* newTour=new Tour(currentTour);//get random neighbor state
			double currentDis=currentTour->getDis(),newDis=newTour->getDis();
			if(accept(currentDis,newDis,tempe)){
				Tour* delTour=currentTour;
				currentTour=newTour;
				if(delTour!=bestTour)
					delete(delTour);
				if(newDis<bestTour->getDis()){
					delTour=bestTour;
					bestTour=newTour;
					delete(delTour);
					bestIte=ite;
				}
				//cout<<"Ite: "<<ite<<"  Dis: "<<newDis<<endl;
			}
			else
				delete(newTour);
			tempe*=(1-coolingrate);
			//tempe-=coolingrate;

		}
		if(currentTour!=bestTour)
			delete(currentTour);
		printBestTour(bestIte);
	}
	bool accept(double currentDis,double newDis,double tempe){
		if(currentDis>newDis)
			return true;
		double expo=exp((currentDis-newDis)/tempe);
		if(double(rand()%10000)<=expo*10000)
			return true;
		else
			return false;
	}
	void printBestTour(int bestIte){
		cout<<"Best tour dis: "<<bestTour->getDis()<<" ite: "<<bestIte<<endl<<endl;//*2006.82/156.6
		for(int i=0;i<bestTour->getTourSize();i++){
			bestTour->getCity(i)->printCity();
			cout<<"->";
		}
		cout<<endl<<endl;
		delete(bestTour);
		bestTour=NULL;
	}
	void nestedSASearch(){
		for(int i=0;i<15;i++)
			simulatedAnnealingSearch();
	}
};

int main(){
	srand (unsigned(919));
	SimulatedAnnealing sa;
	int i;
	sa.nestedSASearch();
	system("pause");
}