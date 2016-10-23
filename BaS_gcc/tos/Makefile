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
	@(cd jtagwait; make clean)
	@(cd bascook; make clean)
	@(cd vmem_test; make clean)
	@(cd pci_test; make clean)
	@(cd fpga_test; make clean)


