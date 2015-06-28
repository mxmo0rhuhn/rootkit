# Linux rootkit
Term paper for the Zurich university of applied sciences seminar in network security.

This project is a very basic implementation of a Linux rootkit.
Main objectives were: 
- Hiding on the disk (can not be found by ls)
- Hiding in the process table (can not be found by ps)

Therefore a system_call hijack was written based on Tyler Borland's great [tutorial](http://turbochaos.blogspot.ch/2013/09/linux-rootkits-101-1-of-3.html).

## License
This project is free software: You can redistribute it and/or modify it under the terms of the European Union Public Licence (EUPL v.1.1) or – as soon they will be approved by the European Commission - subsequent versions of the EUPL (the "Licence"); You may not use this work except in compliance with the Licence.

The project is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. 
See the Licence for the specific language governing permissions and limitations under the Licence.
