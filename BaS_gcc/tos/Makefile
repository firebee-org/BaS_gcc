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

