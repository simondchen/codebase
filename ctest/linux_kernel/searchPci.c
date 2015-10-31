#include <linux/pci.h>
#include <linux/list.h>
void searchPci(list_head *bus){
	list_head *bus,dev;	
	struct pci_dev *pci;
	struct pci_bus *bus;
	int count=0;
	//采用深度优先遍历
	//如果是空指针，直接返回
	if(bus==NULL){
		return;
	}
	if(bus->next=bus){
		return;
	}
	list_for_each(list,bus){	
		bus=list_entry(list,struct pci_bus,node);
		if(bus->devices.next==&bus->devices){	
			return;
		}
		//先遍历bus上的device
		list_for_each(list,&bus->devices){
			pci=list_entry(list,struct pci_dev,bus_list);
			count++;
		}
		if(bus->children.next!=&bus->children){
			list_for_each(list,&bus->children){
				bus=list_entry(list,struct pci_bus,node);
				searchPci(list);
			}
		}
	}
}

int main(void){
	searchPci(&pci_root_buses);
}
