//S Middleton - March 19
//Class to store the summations which for elements of gamma and beta matrices for straight track cosmic fits
#include <cstdio>
#include <math.h>
#include<exception>
#include "DataProducts/inc/XYZVec.hh"
#include "Mu2eUtilities/inc/BuildMatrixSums.hh"

BuildMatrixSums::BuildMatrixSums() {
  clear();
}

BuildMatrixSums::BuildMatrixSums(const BuildMatrixSums& S) {
  init(S);
 
}

BuildMatrixSums::~BuildMatrixSums() {
}
void BuildMatrixSums::clear(){
 
  betaX00 = 0;
  betaX10 = 0;
  gammaX00 = 0;
  gammaX01 = 0;
  gammaX11 = 0;
  betaY00 =  0;
  betaY10 =  0;
  gammaY00 = 0;
  gammaY01 = 0;
  gammaY11 = 0;
  deltaX = 0;
  deltaY = 0;
  chi2 = 0;
}
void BuildMatrixSums::init(const BuildMatrixSums& S) {
 
  betaX00 = S.betaX00;
  betaX10 = S.betaX10;
  gammaX00 = S.gammaX00;
  gammaX01 = S.gammaX01;
  gammaX11 = S.gammaX11;
  betaY00 =  S.betaY00;
  betaY10 =  S.betaY10;
  gammaY00 = S.gammaY00;
  gammaY01 = S.gammaY01;
  gammaY11 = S.gammaY11;
  deltaX = S.deltaX;
  deltaY = S.deltaY;
  chi2 = S.chi2;
}

void BuildMatrixSums::addPoint(int i, XYZVec point_i, XYZVec XPrime, XYZVec YPrime, XYZVec ZPrime,  double errX, double errY){
	XYZVec h_Prime(point_i.Dot(XPrime), point_i.Dot(YPrime),point_i.Dot(ZPrime));
	
        if(errX == 0) return;
        if(errY ==0) return;
        
	betaX00 += (h_Prime.x()/pow(errX,2));
        betaX10 += (h_Prime.z()*h_Prime.x()*(1/pow(errX,2)));
        
	//For GammaX:
	gammaX00 +=(1/pow(errX,2));
        gammaX01 += (h_Prime.z()*(1/pow(errX,2)));
	gammaX11 += (pow(h_Prime.z(),2)*(1/pow(errX,2)));
	
	//For BetaY:
	betaY00 += (h_Prime.y()/pow(errY,2));
        betaY10 += (h_Prime.z()*h_Prime.y()*(1/pow(errY,2)));
        
	//For GammaX:
	
	gammaY00 +=(1/pow(errY,2));	
        gammaY01 += (h_Prime.z()*(1/pow(errY,2)));
	gammaY11 += (pow(h_Prime.z(),2)*(1 /pow(errY,2)));
	
        //Deltas:
        deltaX += (pow(h_Prime.x(),2)*(1/pow(errX,2)));
	deltaY += (pow(h_Prime.y(),2)*(1/pow(errY,2)));
	
	
}


double BuildMatrixSums::Get2DParameter(int i, TMatrixD Alpha){
	return Alpha[i][0];

}

void BuildMatrixSums::SetChi2(double newchi ){
	chi2 = newchi;
	
}


TMatrixD BuildMatrixSums::GetGammaX(){
	TMatrixD Gamma(2,2);
	Gamma[0][0] = gammaX00;
	Gamma[1][0] = gammaX01;
	Gamma[0][1] = gammaX01;
	Gamma[1][1] = gammaX11;
	
	return Gamma;
}

TMatrixD BuildMatrixSums::GetBetaX(){
	TMatrixD Beta(2,1);
	Beta[0][0] = betaX00;
	Beta[1][0] = betaX10;
	
	return Beta;
}

TMatrixD BuildMatrixSums::GetAlphaX(){
        TMatrixD Gamma = GetGammaX();
	TMatrixD Beta  = GetBetaX();
	double* det = NULL;                  
	try {
    	        Gamma.Invert(det);
    
  	   } catch (std::exception exc) {
    
		    std::stringstream message;
		    message << "Error in Alpha : Cannot fit due to singular matrix error on inversion!" << std::endl;
          } 
        TMatrixD Alpha(Gamma*Beta);
       
	return Alpha;
}

TMatrixD BuildMatrixSums::GetGammaY(){
	TMatrixD Gamma(2,2);
	Gamma[0][0] = gammaY00;
	Gamma[1][0] = gammaY01;
	Gamma[0][1] = gammaY01;
	Gamma[1][1] = gammaY11;
	
	return Gamma;
}

TMatrixD BuildMatrixSums::GetBetaY(){
	TMatrixD Beta(2,1);
	Beta[0][0] = betaY00;
	Beta[1][0] = betaY10;
	
	return Beta;
}

TMatrixD BuildMatrixSums::GetAlphaY(){
        TMatrixD Gamma = GetGammaY();
	TMatrixD Beta  = GetBetaY();
	double* det = NULL; 
	                 
	try {
    	        Gamma.Invert(det);
    
  	   } catch (std::exception exc) {
    
		    std::stringstream message;
		    message << "Error in Alpha : Cannot fit due to singular matrix error on inversion!" << std::endl;
          } 
        TMatrixD Alpha(Gamma*Beta);
       return Alpha;
}

double BuildMatrixSums::GetChi2X(){
	TMatrixD GammaX = GetGammaX();
	TMatrixD BetaX  = GetBetaX();
	//TMatrixD AlphaX = GetAlphaX();
        double* det = NULL;                  
	try {
    	        GammaX.Invert(det);
  	   } catch (std::exception exc) {
		    std::stringstream message;
		    message << "Error in Chi2X : Cannot fit due to singular matrix error on inversion!" << std::endl;
          } 
        TMatrixD DeltaX(1,1);
        DeltaX[0][0] = deltaX;   
	TMatrixD Betat(BetaX);
        Betat.T(); 
        TMatrixD Gammat(GammaX);
        Gammat.T(); 
	TMatrixD chi2X(DeltaX-(Betat*Gammat*BetaX));
	
	return chi2X[0][0];
}

double BuildMatrixSums::GetChi2Y(){

	TMatrixD GammaY = GetGammaY();
	TMatrixD BetaY  = GetBetaY();
        double* det = NULL;                  
	try {
    	        GammaY.Invert(det);
    
  	   } catch (std::exception exc) {
    
		    std::stringstream message;
		    message << "Error in Chi2Y : Cannot fit due to singular matrix error on inversion!" << std::endl;
        } 
       
        TMatrixD DeltaY(1,1);
        DeltaY[0][0] = deltaY;	
        TMatrixD Betat(BetaY);
        Betat.T(); 	
        TMatrixD Gammat(GammaY);       
        Gammat.T(); 
	TMatrixD chi2Y(DeltaY-(Betat*Gammat*BetaY));	
	return chi2Y[0][0];
}

double BuildMatrixSums::GetTotalChi2(){
	double chi2X = GetChi2X();
        double chi2Y = GetChi2Y();
        return chi2Y+chi2X;
}




