##
 # Licensed under the Apache License, Version 2.0 (the "License");
 # you may not use this file except in compliance with the License.
 # You may obtain a copy of the License at
 #
 # http://www.apache.org/licenses/LICENSE-2.0
 #
 # Unless required by applicable law or agreed to in writing, software
 # distributed under the License is distributed on an "AS IS" BASIS,
 # WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 # See the License for the specific language governing permissions and
 # limitations under the License.
 ##

'''
Mapping TwisterX C++ Comm Type with PyTwisterX CommType
'''

from pytwisterx.net.comm_type cimport _CommType

cpdef enum CommType:
    MPI = _CommType._MPI
    TCP = _CommType._TCP
    UCX = _CommType._UCX