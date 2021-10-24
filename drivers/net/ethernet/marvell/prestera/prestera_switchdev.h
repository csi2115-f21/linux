/* SPDX-License-Identifier: BSD-3-Clause OR GPL-2.0 */
/* Copyright (c) 2019-2020 Marvell International Ltd. All rights reserved. */

#ifndef _PRESTERA_SWITCHDEV_H_
#define _PRESTERA_SWITCHDEV_H_

int prestera_switchdev_init(struct prestera_switch *sw);
void prestera_switchdev_fini(struct prestera_switch *sw);

<<<<<<< HEAD
int prestera_bridge_port_event(struct net_device *dev, unsigned long event,
			       void *ptr);
=======
int prestera_bridge_port_join(struct net_device *br_dev,
			      struct prestera_port *port);

void prestera_bridge_port_leave(struct net_device *br_dev,
				struct prestera_port *port);
>>>>>>> parent of 515dcc2e0217... Merge tag 'dma-mapping-5.15-2' of git://git.infradead.org/users/hch/dma-mapping

#endif /* _PRESTERA_SWITCHDEV_H_ */
