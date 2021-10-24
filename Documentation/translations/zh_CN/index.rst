.. raw:: latex

	\renewcommand\thesection*
	\renewcommand\thesubsection*
<<<<<<< HEAD
=======
	\kerneldocCJKon

.. _linux_doc_zh:
>>>>>>> parent of 515dcc2e0217... Merge tag 'dma-mapping-5.15-2' of git://git.infradead.org/users/hch/dma-mapping

中文翻译
========

这些手册包含有关如何开发内核的整体信息。内核社区非常庞大，一年下来有数千名开发
人员做出贡献。 与任何大型社区一样，知道如何完成任务将使得更改合并的过程变得更
加容易。

<<<<<<< HEAD
翻译计划:
内核中文文档欢迎任何翻译投稿，特别是关于内核用户和管理员指南部分。
=======
.. note::

   **翻译计划:**
   内核中文文档欢迎任何翻译投稿，特别是关于内核用户和管理员指南部分。

许可证文档
----------

下面的文档介绍了Linux内核源代码的许可证（GPLv2）、如何在源代码树中正确标记
单个文件的许可证、以及指向完整许可证文本的链接。

* Documentation/translations/zh_CN/process/license-rules.rst

用户文档
--------

下面的手册是为内核用户编写的——即那些试图让它在给定系统上以最佳方式工作的
用户。
>>>>>>> parent of 515dcc2e0217... Merge tag 'dma-mapping-5.15-2' of git://git.infradead.org/users/hch/dma-mapping

.. toctree::
   :maxdepth: 2

   admin-guide/index
   process/index
   filesystems/index
<<<<<<< HEAD
=======

TODOList:

* driver-api/index
* locking/index
* accounting/index
* block/index
* cdrom/index
* ide/index
* fb/index
* fpga/index
* hid/index
* i2c/index
* isdn/index
* infiniband/index
* leds/index
* netlabel/index
* networking/index
* pcmcia/index
* power/index
* target/index
* timers/index
* spi/index
* w1/index
* watchdog/index
* virt/index
* input/index
* hwmon/index
* gpu/index
* security/index
* crypto/index
* vm/index
* bpf/index
* usb/index
* PCI/index
* scsi/index
* misc-devices/index
* scheduler/index
* mhi/index

体系结构无关文档
----------------

TODOList:

* asm-annotations

特定体系结构文档
----------------

.. toctree::
   :maxdepth: 2

   mips/index
>>>>>>> parent of 515dcc2e0217... Merge tag 'dma-mapping-5.15-2' of git://git.infradead.org/users/hch/dma-mapping
   arm64/index

目录和表格
----------

* :ref:`genindex`
