.PHONY: tos
.PHONY: jtagwait
.PHONY: bascook
.PHONY: vmem_test
.PHONY: pci_test
.PHONY: pci_mem
.PHONY: fpga_test
tos: jtagwait bascook vmem_test pci_test pci_mem fpga_test

jtagwait:
	@$(MAKE) -s -C $@

bascook:
	@$(MAKE) -s -C $@

vmem_test:
	@$(MAKE) -s -C $@

pci_test:
	@$(MAKE) -s -C $@

pci_mem:
	@$(MAKE) -s -C $@

fpga_test:
	@$(MAKE) -s -C $@

.PHONY: clean
clean:
	@(cd jtagwait; $(MAKE) -s clean)
	@(cd bascook; $(MAKE) -s clean)
	@(cd vmem_test; $(MAKE) -s clean)
	@(cd pci_test; $(MAKE) -s clean)
	@(cd pci_mem; $(MAKE) -s clean)
	@(cd fpga_test; $(MAKE) -s clean)


