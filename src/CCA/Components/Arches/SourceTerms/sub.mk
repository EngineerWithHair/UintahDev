# Makefile fragment for this subdirectory

SRCDIR   := CCA/Components/Arches/SourceTerms

SRCS += \
				$(SRCDIR)/SourceTermFactory.cc \
				$(SRCDIR)/SourceTermBase.cc \
				$(SRCDIR)/CoalGasDevol.cc \
				$(SRCDIR)/CoalGasDevolMom.cc \
				$(SRCDIR)/CoalGasOxi.cc \
				$(SRCDIR)/CoalGasOxiMom.cc \
				$(SRCDIR)/CoalGasHeat.cc \
				$(SRCDIR)/ConstSrcTerm.cc \
				$(SRCDIR)/UnweightedSrcTerm.cc \
				$(SRCDIR)/WestbrookDryer.cc \
				$(SRCDIR)/CoalGasMomentum.cc \
				$(SRCDIR)/Inject.cc \
				$(SRCDIR)/TabRxnRate.cc \
				$(SRCDIR)/IntrusionInlet.cc \
				$(SRCDIR)/DORadiation.cc \
				$(SRCDIR)/RMCRT.cc \
				$(SRCDIR)/BowmanNOx.cc \
       	$(SRCDIR)/BrownSootFormation_nd.cc \
      	$(SRCDIR)/BrownSootFormation_rhoYs.cc \
      	$(SRCDIR)/BrownSootFormation_Tar.cc \
	$(SRCDIR)/SootMassBalance.cc \
				$(SRCDIR)/PCTransport.cc \
				$(SRCDIR)/MMS1.cc \
				$(SRCDIR)/SecondMFMoment.cc \
				$(SRCDIR)/ManifoldRxn.cc \
				$(SRCDIR)/DissipationSource.cc \
				$(SRCDIR)/ShunnMoinMMSMF.cc \
				$(SRCDIR)/ShunnMoinMMSCont.cc \
				$(SRCDIR)/MomentumDragSrc.cc



