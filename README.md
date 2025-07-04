# UdsDoIPServer
Integration of Unified Diagnostic Services (UDS) and Diagnostics over IP (DoIP)

### Projects been integrated

1. [libdoip](https://github.com/AVL-DiTEST-DiagDev/libdoip/tree/dev)
```
https://github.com/AVL-DiTEST-DiagDev/libdoip/tree/dev
```

2. [iso14229](https://github.com/driftregion/iso14229/tree/v0.8.1)
```
https://github.com/driftregion/iso14229/releases/tag/v0.8.1
```

### Integration

This project forked libdoip project and used it as the basis, then added iso14229 project's v0.8.1 release files. Fixed some errors and added glue logic to integrate two projects together. 

To create a complete UDS server over DoIP, you just need to add UDS service handlers to UdsDoIPServer folder. Currently, there's a simple DID handler example module in that folder.

The original libdoip README file has been renamed to README.libdoip.md.
