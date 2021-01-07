//

smooth* s;
TGraph* gr;
TH1F*   h;

//-----------------------------------------------------------------------------
// test initialization starting from TGraph
//-----------------------------------------------------------------------------
int smooth_test_graph() {

  double data[] = {
    10,   0.1481,
    20,   0.5021,
    30,   0.9783,
    40,   1.525,
    50,   2.099,
    60,   2.654,
    70,   3.144,
    80,   3.490,
    90,   3.435,
    100,   2.009,
    110,   3.098e-1,
    120,   2.550e-2,
    130,   2.531e-3,
    140,   3.144e-4,
    150,   4.368e-5,
    160,   6.043e-6,
    170,   7.296e-7,
    180,   6.231e-8,
    190,   2.289e-9,
    200,   3.285e-12,
    -1};

  double x[10000], y[10000];
  
  int np = 0;

  while (data[2*np] > 0) {
    x[np] = data[2*np  ];
    y[np] = data[2*np+1];

    printf ("np = %2i x[np], y[np] = %12.5e %12.5e\n",np,x[np],y[np]);
    np++;
  }

  gr = new TGraph(np,x,y);

  gr->SetLineColor(kRed+2);
  gr->SetMarkerStyle(20);
  gr->SetMarkerSize(1);
  gr->Draw("alp");

  s = new smooth(gr);

  return 0;
}

//-----------------------------------------------------------------------------
// test initialization starting from TH1F
//-----------------------------------------------------------------------------
int smooth_test_hist() {

  double data[] = {
    10,   0.1481,
    20,   0.5021,
    30,   0.9783,
    40,   1.525,
    50,   2.099,
    60,   2.654,
    70,   3.144,
    80,   3.490,
    90,   3.435,
    100,   2.009,
    110,   3.098e-1,
    120,   2.550e-2,
    130,   2.531e-3,
    140,   3.144e-4,
    150,   4.368e-5,
    160,   6.043e-6,
    170,   7.296e-7,
    180,   6.231e-8,
    190,   2.289e-9,
    200,   3.285e-12,
    -1};


  h = new TH1F("h1","h1",20,5,205);

  for (int i=0; i<20; i++) {
    h->SetBinContent(i+1,data[2*i+1]);
  }

  h->SetLineColor(kRed+2);
  h->SetMarkerStyle(20);
  h->SetMarkerSize(1);
  h->Draw("");

  s = new smooth(h);

  s->GetFunc()->Draw("same");
 
  return 0;
}
