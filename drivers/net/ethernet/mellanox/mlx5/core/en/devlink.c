// SPDX-License-Identifier: GPL-2.0 OR Linux-OpenIB
/* Copyright (c) 2020, Mellanox Technologies inc.  All rights reserved. */

#include "en/devlink.h"

int mlx5e_devlink_port_register(struct mlx5e_priv *priv)
{
	struct devlink *devlink = priv_to_devlink(priv->mdev);
	struct devlink_port_attrs attrs = {};

	if (mlx5_core_is_pf(priv->mdev)) {
		attrs.flavour = DEVLINK_PORT_FLAVOUR_PHYSICAL;
		attrs.phys.port_number = PCI_FUNC(priv->mdev->pdev->devfn);
	} else {
		attrs.flavour = DEVLINK_PORT_FLAVOUR_VIRTUAL;
	}

	devlink_port_attrs_set(&priv->dl_port, &attrs);

	return devlink_port_register(devlink, &priv->dl_port, 1);
}

void mlx5e_devlink_port_type_eth_set(struct mlx5e_priv *priv)
{
	devlink_port_type_eth_set(&priv->dl_port, priv->netdev);
}

void mlx5e_devlink_port_unregister(struct mlx5e_priv *priv)
{
<<<<<<< HEAD
	devlink_port_unregister(&priv->dl_port);
=======
	struct devlink_port *dl_port = mlx5e_devlink_get_dl_port(priv);

	if (dl_port->registered)
		devlink_port_unregister(dl_port);
>>>>>>> parent of 515dcc2e0217... Merge tag 'dma-mapping-5.15-2' of git://git.infradead.org/users/hch/dma-mapping
}

struct devlink_port *mlx5e_get_devlink_port(struct net_device *dev)
{
	struct mlx5e_priv *priv = netdev_priv(dev);
	struct devlink_port *port;

<<<<<<< HEAD
	return &priv->dl_port;
=======
	if (!netif_device_present(dev))
		return NULL;
	port = mlx5e_devlink_get_dl_port(priv);
	if (port->registered)
		return port;
	return NULL;
>>>>>>> parent of 515dcc2e0217... Merge tag 'dma-mapping-5.15-2' of git://git.infradead.org/users/hch/dma-mapping
}
