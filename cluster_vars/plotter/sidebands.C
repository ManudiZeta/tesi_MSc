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

static TString file_str_1= "../../../../160326_nog_uubar_chunk1234_tot.root";
//static TString file_str_1= "180326_nog_qqbar_tot.root";

void sidebands()
{
    
    gStyle->SetOptStat(0);
    TFile *myf_1 = new TFile(file_str);
    TTree *tree = (TTree*)myf_1->Get("tree");
    
    double alpha_num = 0.10;
    TString alpha_str= "0.10";
    
    TString signal = "(recoil_mass>=0.8 && recoil_mass<=1.2)";
    TString sidebands = "((recoil_mass >=0.3 && recoil_mass<=0.7) || (recoil_mass>=1.3 && recoil_mass<=1.7))";
    
    tree->Draw("recoil_mass>>mtot(100,0,2)","alpha<" + alpha_str,"goff");
    tree->Draw("recoil_mass>>msig(100,0,2)",signal +  "&& alpha<" + alpha_str,"goff");
    tree->Draw("recoil_mass>>mbkg(100,0,2)", sidebands + "&& alpha<" + alpha_str,"goff");
    
    TH1 *mtot = (TH1*)gDirectory->Get("mtot");
    TH1 *msig = (TH1*)gDirectory->Get("msig");
    TH1 *mbkg = (TH1*)gDirectory->Get("mbkg");
    
    mtot->SetLineColor(kBlack);
    mtot->SetFillColorAlpha(kBlack, 0.3);
    msig->SetLineColor(kRed);
    mbkg->SetLineColor(kBlue);
    
    mtot->GetXaxis()->SetTitle("recoil mass #frac{GeV}{c^{2}}");
    mtot->GetYaxis()->SetTitle("counts");

    
    TLegend *leg = new TLegend(0.1,0.8,0.38,0.9);
    leg->AddEntry(mtot,"all","l");
    leg->AddEntry(msig,"signal region","l");
    leg->AddEntry(mbkg,"sidebands","l");
    
    TCanvas *canv = new TCanvas("tela", "tela");
    
    mtot->DrawCopy("HIST");
    msig->DrawCopy("SAME");
    mbkg->DrawCopy("SAME");
    leg->Draw("SAME");
    
    TString title_out = "recoil_mass_"+ alpha_str + ".pdf";
    canv->SaveAs(title_out);
    
    TString var[8] = {"clusterE","clusterLAT","clusterNHits","clusterSecondMoment","clusterE1E9","clusterE9E21","clusterAbsZernikeMoment51","clusterAbsZernikeMoment40"};
    TString lim [8] = {"(100,0,3)","(256,0,1)","(512,0,80)","(512,0,40)","(256,0,1)","(256,0,1)","(256,0,1.2)","(256,0,1.7)"};
    TString um [8] = {" [GeV]","","","","","","",""};

    
    for(int i = 0; i<8; i++)
    {
        TCanvas *c1 = new TCanvas("c1", "c1",800,600);
        
        tree->Draw("nbar_" + var[i] + ">>h1" + lim[i],signal + " && alpha<" + alpha_str ,"goff");
        tree->Draw("nbar_" + var[i] + ">>h2" + lim[i],sidebands + " && alpha<" + alpha_str,"goff");
        
        
        TH1 *h1 = (TH1*)gDirectory->Get("h1");
        TH1 *h2 = (TH1*)gDirectory->Get("h2");
        h2->Scale(0.5);
        cout<<"h1 entries: "<<h1->GetEntries()<<endl;
        cout<<"h2 entries: "<<h2->GetEntries()<<endl;
        
        delete c1;
        
        h1->Add(h2, -1.0);
        h1->SetLineColor(kGreen);
        //h2->SetLineColor(kRed);
        //f->SetLineColor(kRed);
        
        h1->GetXaxis()->SetTitle(var[i] + um[i]);
        h1->GetYaxis()->SetTitle("counts");
        
        
        TString title = "cut alpha < " + alpha_str;
        h1->SetTitle(title);
        
        TLegend *leg = new TLegend(0.1,0.8,0.38,0.9);
        leg->AddEntry(h1,"Central - sidebands","l");
        //leg->AddEntry(h2,"(0-0.8) U (1.2-2) GeV","l");
        
        
        TCanvas *tela = new TCanvas("tela", "tela");
        
        h1->SetMinimum(0);
        h1->DrawCopy("HISTO");
        //h2->DrawCopy("HISTO SAME");
        //f->Draw("SAME");
        leg->Draw("SAME");
        
        TString title_out = "sidebands_" + var[i] + "_" + alpha_str + "_subtract.pdf";
        tela->SaveAs(title_out);
        
        delete tela;
        delete h1;
        delete h2;
        
        
    }
    
}

void recoil_mass()
{
    
    
    gStyle->SetOptStat(1110);
    
    double alpha_num [3]  = {0.35,0.10,0.05};
    
    TString alpha_str [3] = {"0.35","0.10","0.05"};
    
    TString var[1] = {"recoil_mass"};
    TString lim [1] = {"(100,0,2)"};
    TString um [1] = {"[#frac{GeV}{c^{2}}]"};
    TFile *myf_1 = new TFile(file_str);
    
    for(int j = 0; j<3; j ++)
    {
        for(int i = 0; i<1; i++)
        {
            TTree *tree = (TTree*)myf_1->Get("tree");
            
            TCanvas *c1 = new TCanvas("c1", "c1",800,600);
            
            tree->Draw(var[i] + ">>h1" + lim[i],"alpha<" + alpha_str[j] ,"goff");
            
            TH1 *h1 = (TH1*)gDirectory->Get("h1");
            cout<<"h1 entries: "<<h1->GetEntries()<<endl;
            
            delete c1;
            
            h1->SetLineColor(kBlue);
            //f->SetLineColor(kRed);
            
            h1->GetXaxis()->SetTitle(var[i] + um[i]);
            h1->GetYaxis()->SetTitle("counts");
            
            
            TString title = "cut alpha < " + alpha_str[j];
            h1->SetTitle(title);
            
            TLegend *leg = new TLegend(0.1,0.8,0.48,0.9);
            leg->AddEntry(h1,"recoil mass in [0-2] GeV/c^{2}","l");
            //leg->AddEntry(h2,"(0-0.8) U (1.2-2)","l");
            
            
            TCanvas *tela = new TCanvas("tela", "tela");
            
            h1->DrawCopy("HIST");
            //h2->DrawCopy("SAME");
            //f->Draw("SAME");
            //leg->Draw("SAME");
            
            TString title_out = "images/" + var[i] + "_" + alpha_str[j] + ".pdf";
            tela->SaveAs(title_out);
            
            delete tree;
            delete tela;
        }
    }
}


void cluster_vars()
{
    
    
    gStyle->SetOptStat(0);
    
    double alpha_num [10]  = {0.35,0.30,0.25,0.20,0.15,0.10,0.075,0.05,0.04,0.03};
    
    TString alpha_str [10] = {"0.35","0.30","0.25","0.20","0.15","0.10","0.075","0.05","0.04","0.03"};
    
    TString var[8] = {"clusterE","clusterLAT","clusterNHits","clusterSecondMoment","clusterE1E9","clusterE9E21","clusterAbsZernikeMoment51","clusterAbsZernikeMoment40"};
    TString lim [8] = {"(100,0,3)","(256,0,1)","(512,0,80)","(512,0,40)","(256,0,1)","(256,0,1)","(256,0,1.2)","(256,0,1.7)"};
    TString um [8] = {" [GeV]","","","","","","",""};
    TFile *myf_1 = new TFile(file_str);
    
    for(int j = 5; j<6; j ++)
    {
        for(int i = 0; i<8; i++)
        {
            TTree *tree = (TTree*)myf_1->Get("tree");
            
            TCanvas *c1 = new TCanvas("c1", "c1",800,600);
            
            tree->Draw("nbar_" + var[i] + ">>h1" + lim[i],"(recoil_mass>=0.8 || recoil_mass<=1.2) && alpha<" + alpha_str[j] ,"goff");
            tree->Draw("nbar_" + var[i] + ">>h2" + lim[i],"(recoil_mass<0.8 || recoil_mass>1.2) && alpha<" + alpha_str[j],"goff");
            
            
            TH1 *h1 = (TH1*)gDirectory->Get("h1");
            TH1 *h2 = (TH1*)gDirectory->Get("h2");
            cout<<"h1 entries: "<<h1->GetEntries()<<endl;
            cout<<"h2 entries: "<<h2->GetEntries()<<endl;
            
            delete c1;
            
            
            h1->SetLineColor(kRed);
            //h2->SetLineColor(kRed);
            //f->SetLineColor(kRed);
            
            h1->GetXaxis()->SetTitle(var[i] + um[i]);
            h1->GetYaxis()->SetTitle("counts");
            
            
            TString title = "cut alpha < " + alpha_str[j];
            h1->SetTitle(title);
            
            TLegend *leg = new TLegend(0.1,0.8,0.38,0.9);
            leg->AddEntry(h1,"recoil mass in [0.8-1.2] GeV","l");
            //leg->AddEntry(h2,"(0-0.8) U (1.2-2)","l");
            
            
            TCanvas *tela = new TCanvas("tela", "tela");
            
            h1->DrawCopy("HIST");
            //h2->DrawCopy("SAME");
            //f->Draw("SAME");
            leg->Draw("SAME");
            
            TString title_out = var[i] + "_" + alpha_str[j] + ".pdf";
            tela->SaveAs(title_out);
            
            delete tree;
            delete tela;
        }
    }
}

void stacks()
{
    
    
    gStyle->SetOptStat(0);
    TFile *myf_1 = new TFile(file_str);
    
    
    TTree *tree = (TTree*)myf_1->Get("tree");
    
    TCanvas *c1 = new TCanvas("c1", "c1",800,600);
    
    tree->Draw("recoil_mass>>h1(100,0,2)","!(!(TMath::IsNaN(nbar_mcPDG)) && (nbar_mcPDG == -2112 || nbar_hasAncestor_2112_1 != 0))","goff");
    tree->Draw("recoil_mass>>h2(100,0,2)","!(TMath::IsNaN(nbar_mcPDG)) && (nbar_mcPDG == -2112 || nbar_hasAncestor_2112_1 != 0) ","goff");
    
    
    
    THStack *hs = new THStack("hs","");
    TH1 *h1 = (TH1*)gDirectory->Get("h1");
    TH1 *h2 = (TH1*)gDirectory->Get("h2");
    
    hs->Add(h1);
    hs->Add(h2);
    
    cout<<"h1 entries: "<<h1->GetEntries()<<endl;
    cout<<"h2 entries: "<<h2->GetEntries()<<endl;
    
    
    //TF1 *f = new TF1("f_cheb","chebyshev2",0,2,3);
    //h1->Fit("f_cheb","R");
    
    delete c1;
    
    
    h1->SetLineColor(kBlue);
    h2->SetLineColor(kRed);
    //f->SetLineColor(kRed);
    
    h1->GetXaxis()->SetTitle("recoil mass #frac{GeV}{c^{2}}");
    h1->GetYaxis()->SetTitle("counts");
    
    
    TString title = "";
    h1->SetTitle(title);
    
    TLegend *leg = new TLegend(0.1,0.8,0.38,0.9);
    leg->AddEntry(h1,"all entries","l");
    leg->AddEntry(h2,"signal","l");
    
    
    
    TCanvas *tela = new TCanvas("tela", "tela");
    
    hs->Draw();
    leg->Draw("SAME");
    
    TString title_out = "images/stack_mRecoil_isSignal.pdf";
    tela->SaveAs(title_out);
    
    delete tree;
    delete tela;
    
}

void fom()
{
    gStyle->SetOptStat(0);
    TFile *myf_1 = new TFile(file_str);
    
    
    TTree *tree = (TTree*)myf_1->Get("tree");
    
    double alpha_num [10]  = {0.35,0.30,0.25,0.20,0.15,0.10,0.075,0.05,0.04,0.03};
    double fom_i = 0;
    double fom_rad = 0;
    
    TGraphErrors *gr = new TGraphErrors();
    gr->SetTitle("fom vs alpha");
    gr->GetXaxis()->SetTitle("alpha [rad]");
    gr->GetYaxis()->SetTitle("#frac{signal}{#sqrt{tot}}");
    
    TString alpha_str [10] = {"0.35","0.30","0.25","0.20","0.15","0.10","0.075","0.05","0.04","0.03"};
    
    for (int i=0; i<10; i++)
    {
        TCanvas *c1 = new TCanvas("c1", "c1",800,600);
        tree->Draw("recoil_mass>>h1(100,0.8,1.2)", "recoil_mass>0.8 && recoil_mass<1.2 && alpha <" + alpha_str[i],"goff");
        tree->Draw("recoil_mass>>h2(100,0.8,1.2)","(nbar_mcPDG == -2112 || nbar_hasAncestor_2112_1 != 0) && recoil_mass>0.8 && recoil_mass<1.2 && alpha <" + alpha_str[i],"goff");
        
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
    
    TString title_out = "images/fom_alpha.pdf";
    tela->SaveAs(title_out);
    
}

void roc()
{
    
    
    gStyle->SetOptStat(0);
    TFile *myf_1 = new TFile(file_str);
    
    
    TTree *tree = (TTree*)myf_1->Get("tree");
    
    double alpha_num [10]  = {0.35,0.30,0.25,0.20,0.15,0.10,0.075,0.05,0.04,0.03};
    double x_i = 0;
    double y_i = 0;
    
    TGraphErrors *gr = new TGraphErrors();
    gr->SetTitle("ROC curve");
    gr->GetXaxis()->SetTitle("purity");
    gr->GetYaxis()->SetTitle("#epsilon");
    
    TString alpha_str [10] = {"0.35","0.30","0.25","0.20","0.15","0.10","0.075","0.05","0.04","0.03"};
    
    for (int i=0; i<10; i++)
    {
        TCanvas *c1 = new TCanvas("c1", "c1",800,600);
        
        tree->Draw("recoil_mass>>h0(100,0.8,1.2)", "recoil_mass>0.8 && recoil_mass<1.2 && alpha < 0.35","goff");
        tree->Draw("recoil_mass>>h1(100,0.8,1.2)", "recoil_mass>0.8 && recoil_mass<1.2 && alpha <" + alpha_str[i],"goff");
        
        tree->Draw("recoil_mass>>h2(100,0.8,1.2)","(nbar_mcPDG == -2112 || nbar_hasAncestor_2112_1 != 0) && recoil_mass>0.8 && recoil_mass<1.2 && alpha < 0.35","goff");
        tree->Draw("recoil_mass>>h3(100,0.8,1.2)","(nbar_mcPDG == -2112 || nbar_hasAncestor_2112_1 != 0) && recoil_mass>0.8 && recoil_mass<1.2 && alpha <" + alpha_str[i],"goff");
        
        
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
        latex->SetTextSize(0.03);
        latex->SetTextColor(kBlack);
        latex->SetTextAlign(12);
        
        gr->GetListOfFunctions()->Add(latex);
    }
    
    TCanvas *tela = new TCanvas("tela", "tela");
    gr->Draw("AP*");
    TString title_out = "images/eff_alpha.pdf";
    tela->SaveAs(title_out);
    
    
    //f->Draw("same");
    TF1 *f = new TF1("my_f","pol3",0.75,1);
    gr->Fit("my_f","R");
    double auc = gr->Integral();
    cout<<"AUC: "<<auc;
    
    
}

