void plot_dedx() {
    // Open ROOT file
    TFile *f = TFile::Open("G4Analysis.root");
    if (!f || f->IsZombie()) {
        printf("Error: Could not open file G4Analysis.root\n");
        return;
    }

    // Get histogram
    TH2D *h_kaon = (TH2D*)f->Get("h2_dE_dx_kaon");
    if (!h_kaon) {
        printf("Error: Could not get histogram h2_dE_dx_kaon\n");
        f->ls();
        return;
    }

    // Create canvas
    TCanvas *c = new TCanvas("c", "dE vs dx for K+", 800, 600);
    
    // Configure histogram
    h_kaon->SetStats(1);
    h_kaon->SetTitle("dE vs dx for K+ (50 MeV, 20#circ)");
    h_kaon->GetXaxis()->SetTitle("dx (mm)");
    h_kaon->GetYaxis()->SetTitle("dE (MeV)");
    
    // Draw histogram with color scale
    h_kaon->Draw("COLZ");
    
    // Enable logarithmic scale
    c->SetLogz();
    
    // Add statistics box
    gPad->Update();
    TPaveStats *st = (TPaveStats*)h_kaon->FindObject("stats");
    if (st) {
        st->SetX1NDC(0.7);
        st->SetX2NDC(0.9);
        st->SetY1NDC(0.7);
        st->SetY2NDC(0.9);
    }
    
    // Save plots
    c->SaveAs("kaon_dE_dx.pdf");
    c->SaveAs("kaon_dE_dx.png");
}