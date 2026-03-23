#include "TFile.h"
#include <Riostream.h>
#include "TCanvas.h"
#include "TH1D.h"
#include "TF1.h"
#include "TTree.h"
#include "TH2F.h"
#include "THStack.h"
#include "TMath.h"
#include "TLegend.h"
#include "TStyle.h"
#include "TLine.h"
#include "TString.h"
#include "TLatex.h"
#include "TGraphErrors.h"

//static TString file_str_1= "../../../../160326_nog_uubar_chunk1234_tot.root"; //alpha<0.2, 24,2,16,6
static TString file_str_1= "../../../../180326_nog_qqbar_tot.root";
//static TString file_str_1= "160326_nog_tot.root";
static TString ch1= "2";
static TString ch2= "1";
static TString ch3= "4";
static TString ch4= "9";
void recoil_mass ()
{
    //gStyle->SetOptStat(0);
    gStyle->SetOptStat(0);

    TFile *myf_1 = new TFile(file_str_1);
    TTree *tree_1 = (TTree*)myf_1->Get("tree");
    
    
    TCanvas *c1 = new TCanvas("c1", "c1",800,600);
    tree_1->Draw("recoil_mass>>histo1(100,0,2)");
    
       
    
    delete c1;
    
    TH1D *histo1 = (TH1D*)gDirectory->Get("histo1");

    histo1->SetLineColor(kBlue);
    
    TString title_x = "recoil mass [#frac{GeV}{c^{2}}]";
    histo1->GetXaxis()->SetTitle(title_x);
    histo1->GetYaxis()->SetTitle("counts");
    histo1->SetTitle("");
    
    TCanvas *tela = new TCanvas("tela", "tela");
    
    histo1->DrawCopy("HIST");
    
    TString title_out = "../images/recoil_mass_nog_020.pdf";
    tela->SaveAs(title_out);
    
}


void topo_study()
{
    TFile *f1 = new TFile(file_str_1);
    
    TTree *tree_1 = (TTree*)f1->Get("tree");
    TTree *topo_1 = (TTree*)f1->Get("TOPO");
    tree_1->BuildIndex("__event__");
    topo_1->BuildIndex("__event__");
    tree_1->AddFriend(topo_1);
    
    // signal component
    TCanvas *c1 = new TCanvas("c1", "c1",800,600);
    tree_1->Draw("recoil_mass>>h0(100,0,2)","TOPO.iDcyTr==" + ch1); //p,nbar,pi-

    // first 3 bkg
    tree_1->Draw("recoil_mass>>h1(100,0,2)","TOPO.iDcyTr ==" + ch2);  //p,nbar,pi-,pi0
    tree_1->Draw("recoil_mass>>h2(100,0,2)","TOPO.iDcyTr ==" + ch3); //p,nbar,pi-,gamma_isr
    tree_1->Draw("recoil_mass>>h3(100,0,2)","TOPO.iDcyTr ==" + ch4); //p,nbar,pi-,pi0,gamma_isr
    
    // other background
    tree_1->Draw("recoil_mass>>h4(100,0,2)","TOPO.iDcyTr !=" + ch1 +"&& TOPO.iDcyTr != " + ch2 +" && TOPO.iDcyTr != "+ ch3 +" && TOPO.iDcyTr != "+ ch4);
    
    TH1 *h0 = (TH1*)gDirectory->Get("h0");
    TH1 *h1 = (TH1*)gDirectory->Get("h1");
    TH1 *h2 = (TH1*)gDirectory->Get("h2");
    TH1 *h3 = (TH1*)gDirectory->Get("h3");
    TH1 *h4 = (TH1*)gDirectory->Get("h4");
    
    h0->SetLineColor(kGreen);
    h0->SetFillColor(kGreen);
    
    h1->SetLineColor(kBlue);
    h1->SetFillColor(kBlue);
    
    h2->SetLineColor(kRed);
    h2->SetFillColor(kRed);
    
    h3->SetLineColor(kYellow);
    h3->SetFillColor(kYellow);
    
    h4->SetLineColor(kCyan);
    h4->SetFillColor(kCyan);
        
    delete c1;
    

    THStack *hs = new THStack("hs","");

    hs->Add(h4);
    hs->Add(h3);
    hs->Add(h1);
    hs->Add(h2);
    hs->Add(h0);
    
    //hsum->Scale(1/hsum->GetEntries());
    //hsum_not->Scale(1/hsum_not->GetEntries());
    
    TLegend *leg = new TLegend(0.6,0.6,0.78,0.78);
    leg->AddEntry(h0,"p #bar{n} #pi^{-}","l");
    leg->AddEntry(h1,"p #bar{n} #pi^{-} #pi^{0} ","l");
    leg->AddEntry(h2,"p #bar{n} #pi^{-} #gamma_{ISR} ","l");
    leg->AddEntry(h3,"p #bar{n} #pi^{-} #pi^{0} #gamma_{ISR} ","l");
    leg->AddEntry(h4,"Other bkg ","l");
    TCanvas *tela = new TCanvas("tela", "tela");
    hs->Draw("HISTO");
    //hsum_not->Draw("HISTO SAME");
    leg->Draw("SAME");
    
    hs->GetXaxis()->SetTitle("recoil mass [#frac{GeV}{c^{2}}]");
    hs->GetYaxis()->SetTitle("counts");

    // 4. Aggiorna
    gPad->Update();
    
    TString title_out = "../images/recoilM_qqbar_comps_nog_010.pdf";
    tela->SaveAs(title_out);
}

void risoluz_rec_mass()
{
    gStyle->SetOptStat(1100);
    
    TFile *f1 = new TFile(file_str_1);
    TTree *tree_1 = (TTree*)f1->Get("tree");
    TTree *topo_1 = (TTree*)f1->Get("TOPO");
    tree_1->BuildIndex("__event__");
    topo_1->BuildIndex("__event__");
    tree_1->AddFriend(topo_1);
    
    // signal component
    TCanvas *c1 = new TCanvas("c1", "c1",800,600);
    tree_1->Draw("recoil_mass>>h0(100,0,2)","TOPO.iDcyTr==" + ch1); //" && TOPO.iDcyTr==" + ch3
    
    TH1 *h0 = (TH1*)gDirectory->Get("h0");

    
    h0->SetLineColor(kGreen);
    h0->SetFillColor(kGreen);
 
    delete c1;
    
    //hsum->Scale(1/hsum->GetEntries());
    //hsum_not->Scale(1/hsum_not->GetEntries());
    
    TLegend *leg = new TLegend(0.6,0.6,0.78,0.78);
    leg->AddEntry(h0,"p #bar{n} #pi^{-}","l");
    TCanvas *tela = new TCanvas("tela", "tela");
    h0->Draw("HISTO");
    //hsum_not->Draw("HISTO SAME");
    leg->Draw("SAME");
    h0->GetXaxis()->SetTitle("recoil mass [#frac{GeV}{c^{2}}]");
    h0->GetYaxis()->SetTitle("counts");
    h0->SetTitle("");
    // 4. Aggiorna
    gPad->Update();
    
    TString title_out = "../images/recoilM_comp0_nog_020.pdf";
    tela->SaveAs(title_out);
}



void fom()
{
    gStyle->SetOptStat(0);
    TFile *f1 = new TFile(file_str_1);
    
    TTree *tree_1 = (TTree*)f1->Get("tree");
    TTree *topo_1 = (TTree*)f1->Get("TOPO");
    tree_1->BuildIndex("__event__");
    topo_1->BuildIndex("__event__");
    tree_1->AddFriend(topo_1);
    
    // signal component
    TCanvas *c1 = new TCanvas("c1", "c1",800,600);
    
    double alpha_num [22]  = {0.20, 0.19, 0.18, 0.17, 0.16, 0.15, 0.14, 0.13, 0.12, 0.11, 0.10, 0.09 ,0.08, 0.07, 0.06, 0.05, 0.04, 0.03, 0.02, 0.01, 0.005, 0.};
    double fom_i = 0;
    double fom_rad = 0;
    
    TGraphErrors *gr = new TGraphErrors();
    gr->SetTitle("");
    gr->GetXaxis()->SetTitle("alpha [rad]");
    gr->GetYaxis()->SetTitle("FOM");
    
    TString alpha_str [22] = {"0.20", "0.19", "0.18", "0.17", "0.16", "0.15", "0.14", "0.13", "0.12", "0.11", "0.10", "0.09" ,"0.08", "0.07", "0.06", "0.05", "0.04", "0.03", "0.02", "0.01","0.005", "0."};
    
    for (int i=0; i<22; i++)
    {
        TCanvas *c1 = new TCanvas("c1", "c1",800,600);
        tree_1->Draw("recoil_mass>>h1(100,0.8,1.2)", "recoil_mass>0.8 && recoil_mass<1.2 && alpha <" + alpha_str[i],"goff");
        tree_1->Draw("recoil_mass>>h2(100,0.8,1.2)","(TOPO.iDcyTr==24 ||TOPO.iDcyTr==16) && recoil_mass>0.8 && recoil_mass<1.2 && alpha <" + alpha_str[i],"goff");
        
        TH1 *h1 = (TH1*)gDirectory->Get("h1");
        TH1 *h2 = (TH1*)gDirectory->Get("h2");
        
        double n_tot = h1->GetEntries();
        double n_sig = h2->GetEntries();
        fom_i = n_sig/n_tot;
        fom_rad = n_sig/sqrt(n_tot);
        
        cout<<"********** alpha <"<<alpha_str[i]<<"********** \n";
        cout<<"h1 entries: "<<h1->GetEntries()<<endl;
        cout<<"h2 entries: "<<h2->GetEntries()<<endl;
        cout<<"fom senza radice: "<<fom_i<<endl;
        cout<<"fom con radice: "<<fom_rad<<endl;
        
        
        
        //TF1 *f = new TF1("f_cheb","chebyshev2",0,2,3);
        //h1->Fit("f_cheb","R");
        
        delete c1;
        
        
        h1->SetLineColor(kBlue);
        h2->SetLineColor(kRed);
        
        gr->SetPoint(i, alpha_num[i], fom_rad);
        gr->SetPointError(i, 0., 0.);
        
        delete h1;
        delete h2;
    }
    
    TCanvas *tela = new TCanvas("tela", "tela");
    
    gr->Draw("AP*");
    
    TString title_out = "../images/fom_alpha.pdf";
    tela->SaveAs(title_out);
    
}

void roc()
{
    
    
    gStyle->SetOptStat(0);
    TFile *f1 = new TFile(file_str_1);
    
    TTree *tree_1 = (TTree*)f1->Get("tree");
    TTree *topo_1 = (TTree*)f1->Get("TOPO");
    tree_1->BuildIndex("__event__");
    topo_1->BuildIndex("__event__");
    tree_1->AddFriend(topo_1);
    
    // signal component
    double alpha_num [22]  = {0.20, 0.19, 0.18, 0.17, 0.16, 0.15, 0.14, 0.13, 0.12, 0.11, 0.10, 0.09 ,0.08, 0.07, 0.06, 0.05, 0.04, 0.03, 0.02, 0.01,0.005, 0.};
    double x_i = 0;
    double y_i = 0;
    
    TGraphErrors *gr = new TGraphErrors();
    gr->SetTitle("");
    gr->GetXaxis()->SetTitle("Purity");
    gr->GetYaxis()->SetTitle("#epsilon_{sig}");
    
    TString alpha_str [22] = {"0.20", "0.19", "0.18", "0.17", "0.16", "0.15", "0.14", "0.13", "0.12", "0.11", "0.10", "0.09" ,"0.08", "0.07", "0.06", "0.05", "0.04", "0.03", "0.02", "0.01","0.005", "0."};
    
    for (int i=0; i<20; i++)
    {
        TCanvas *c1 = new TCanvas("c1", "c1",800,600);
        
        tree_1->Draw("recoil_mass>>h0(100,0.8,1.2)", "recoil_mass>0.8 && recoil_mass<1.2 && alpha < 0.20","goff");
        tree_1->Draw("recoil_mass>>h1(100,0.8,1.2)", "recoil_mass>0.8 && recoil_mass<1.2 && alpha <" + alpha_str[i],"goff");
        
        tree_1->Draw("recoil_mass>>h2(100,0.8,1.2)","(TOPO.iDcyTr==24 || TOPO.iDcyTr==16) && recoil_mass>0.8 && recoil_mass<1.2 && alpha < 0.20","goff");
        tree_1->Draw("recoil_mass>>h3(100,0.8,1.2)","(TOPO.iDcyTr==24 || TOPO.iDcyTr==16)  && recoil_mass>0.8 && recoil_mass<1.2 && alpha <" + alpha_str[i],"goff");
        
            
        TH1 *h0 = (TH1*)gDirectory->Get("h0");
        TH1 *h1 = (TH1*)gDirectory->Get("h1");
        TH1 *h2 = (TH1*)gDirectory->Get("h2");
        TH1 *h3 = (TH1*)gDirectory->Get("h3");
        
        double n_tot_35 = h0->GetEntries();
        double n_tot = h1->GetEntries();
        double n_sig_35 = h2->GetEntries();
        double n_sig = h3->GetEntries();
        
        
        //cout<<"h0 entries: "<<h0->GetEntries()<<endl;
        //cout<<"h1 entries: "<<h1->GetEntries()<<endl;
        //cout<<"h2 entries: "<<h2->GetEntries()<<endl;
        //cout<<"h3 entries: "<<h3->GetEntries()<<endl;
        
        delete c1;
        
        
        h1->SetLineColor(kBlue);
        h2->SetLineColor(kRed);
        
        x_i = (n_sig)/(n_tot);
        y_i = n_sig/(n_sig_35);
        
        cout<<"********** alpha <"<<alpha_str[i]<<"********** \n";
        cout<<"Purity = "<<x_i<<endl;
        cout<<"Efficiency = "<<y_i<<endl;
        
        
        gr->SetPoint(i, x_i, y_i);
        gr->SetPointError(i, 0., 0.);
        
        TString label = Form("#alpha = %s", alpha_str[i].Data());
        double x_lab = x_i + 0.002;
        double y_lab = y_i + 0.002;
        
        TLatex *latex = new TLatex(x_lab, y_lab, label);
        latex->SetTextSize(0.015);
        latex->SetTextColor(kBlack);
        latex->SetTextAlign(12);
    
        gr->GetListOfFunctions()->Add(latex);
    }
    gr->GetXaxis()->SetRangeUser(0., 1.01);
    gr->GetYaxis()->SetRangeUser(0., 1.01);
    double auc = gr->Integral();
    cout << "AUC ≈ " << auc << endl;
    TCanvas *tela = new TCanvas("tela", "tela");
    gr->Draw("AP*");
    TString title_out = "../images/eff_alpha.pdf";
    tela->SaveAs(title_out);
    
    /*
    //f->Draw("same");
    TF1 *f = new TF1("my_f","pol3",0.75,1);
    gr->Fit("my_f","R");
    double auc = gr->Integral();
    cout<<"AUC: "<<auc;
    */
    
}

void true_roc()
{

    gStyle->SetOptStat(0);
    TFile *f1 = new TFile(file_str_1);
    
    TTree *tree_1 = (TTree*)f1->Get("tree");
    TTree *topo_1 = (TTree*)f1->Get("TOPO");
    tree_1->BuildIndex("__event__");
    topo_1->BuildIndex("__event__");
    tree_1->AddFriend(topo_1);
    
    // signal component
    double alpha_num [22]  = {0.20, 0.19, 0.18, 0.17, 0.16, 0.15, 0.14, 0.13, 0.12, 0.11, 0.10, 0.09 ,0.08, 0.07, 0.06, 0.05, 0.04, 0.03, 0.02, 0.01,0.005, 0.};
    double x_i = 0;
    double y_i = 0;
    
    TGraphErrors *gr = new TGraphErrors();
    gr->SetTitle("");
    gr->GetXaxis()->SetTitle("#epsilon_{bkg}");
    gr->GetYaxis()->SetTitle("#epsilon_{sig}");
    
    TString alpha_str [22] = {"0.20", "0.19", "0.18", "0.17", "0.16", "0.15", "0.14", "0.13", "0.12", "0.11", "0.10", "0.09" ,"0.08", "0.07", "0.06", "0.05", "0.04", "0.03", "0.02", "0.01","0.005", "0."};
    
    for (int i=0; i<22; i++)
    {
        TCanvas *c1 = new TCanvas("c1", "c1",800,600);
        
        tree_1->Draw("recoil_mass>>h0(100,0.8,1.2)", "recoil_mass>0.8 && recoil_mass<1.2 && alpha < 0.20","goff");
        tree_1->Draw("recoil_mass>>h1(100,0.8,1.2)", "recoil_mass>0.8 && recoil_mass<1.2 && alpha <" + alpha_str[i],"goff");
        
        tree_1->Draw("recoil_mass>>h2(100,0.8,1.2)","(TOPO.iDcyTr==24 || TOPO.iDcyTr==16) && recoil_mass>0.8 && recoil_mass<1.2 && alpha < 0.20","goff");
        tree_1->Draw("recoil_mass>>h3(100,0.8,1.2)","(TOPO.iDcyTr==24 || TOPO.iDcyTr==16)  && recoil_mass>0.8 && recoil_mass<1.2 && alpha <" + alpha_str[i],"goff");
        
            
        TH1 *h0 = (TH1*)gDirectory->Get("h0");
        TH1 *h1 = (TH1*)gDirectory->Get("h1");
        TH1 *h2 = (TH1*)gDirectory->Get("h2");
        TH1 *h3 = (TH1*)gDirectory->Get("h3");
        
        double n_tot_35 = h0->GetEntries();
        double n_tot = h1->GetEntries();
        double n_sig_35 = h2->GetEntries();
        double n_sig = h3->GetEntries();
        
        
        //cout<<"h0 entries: "<<h0->GetEntries()<<endl;
        //cout<<"h1 entries: "<<h1->GetEntries()<<endl;
        //cout<<"h2 entries: "<<h2->GetEntries()<<endl;
        //cout<<"h3 entries: "<<h3->GetEntries()<<endl;
        
        delete c1;
        
        
        h1->SetLineColor(kBlue);
        h2->SetLineColor(kRed);
        
        x_i = (n_tot - n_sig)/(n_tot_35 - n_sig_35);
        y_i = n_sig/(n_sig_35);
        
        cout<<"********** alpha <"<<alpha_str[i]<<"********** \n";
        cout<<"Fake Rate Background = "<<x_i<<endl;
        cout<<"Efficiency = "<<y_i<<endl;
        
        
        gr->SetPoint(i, x_i, y_i);
        gr->SetPointError(i, 0., 0.);
        if(i>9)
        {
            TString label = Form("#alpha < %s", alpha_str[i].Data());
            double x_lab = x_i + 0.005;
            double y_lab = y_i + 0.005;
            
            TLatex *latex = new TLatex(x_lab, y_lab, label);
            latex->SetTextSize(0.025);
            latex->SetTextColor(kBlack);
            latex->SetTextAlign(12);
            
            gr->GetListOfFunctions()->Add(latex);
        }
    }

    TF1 *f = new TF1("my_f","pol3",0,1);
    gr->Fit("my_f","R");
    double auc = f->Integral(0., 1.);
    cout << "AUC ≈ " << auc << endl;
    TCanvas *tela = new TCanvas("tela", "tela");
    gr->GetXaxis()->SetRangeUser(0., 1.01);
    gr->GetYaxis()->SetRangeUser(0., 1.01);
    gr->Draw("AP*");
    TString title_out = "../images/roc_alpha_true.pdf";
    tela->SaveAs(title_out);
    

    
}

