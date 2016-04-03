.PHONY: tos
.PHONY: jtagwait
.PHONY: bascook
.PHONY: vmem_test
.PHONY: pci_test
tos: jtagwait bascook vmem_test pci_test

jtagwait:
	$(MAKE) -C $@

bascook:
	$(MAKE) -C $@

vmem_test:
	$(MAKE) -C $@

pci_test:
	$(MAKE) -C $@

.PHONY: clean
clean:
	(cd jtagwait; make clean)
	(cd bascook; make clean)
	(cd vmem_test; make clean)
	(cd pci_test; make clean)


