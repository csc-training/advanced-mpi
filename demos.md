# Parallel I/O
### Striping example:

```
touch first_file
lfs getstripe first_file

mkdir stripe_exp
lfs setstripe -c 4 experiments
touch experiments/new_file
lfs getstripe experiments/new_file
```
### MPI-IO simple writing
```
 mpif90 mpi_write_at.F90

 srun  --job-name=example --account=project_2002078  --partition=test --time=00:15:00 --ntasks=2 ./a.out

 less test
 hexdump test
 hexdump -e '16 "%02d ""\n"' test
```

### Hdf5 Command Line Tools Example

```
h5ls py_example.hdf5

h5ls py_example.hdf5/DataTwo

h5dump -d /DataTwo/Matrix_two py_example.hdf5

h5dump -n 1 py_example.hdf5

h5dump -a "Matrix_one/maxvalue" py_example.hdf5
h5dump -a "Matrix_one/minvalue" py_example.hdf5
h5dump -a "Matrix_one/average" py_example.hdf5
h5dump -a "Matrix_one/var" py_example.hdf5

h5mkgrp py_example.hdf5 "/DataTwo/test"

h5ls py_example.hdf5/DataTwo
```


### Python hdf5 example
``` 
module load hdf5
module load biokit
module load biopythontools

python # start python
``` 
```
import h5py
import numpy as np

m1_shape=(10,20,4)
m2_shape=(6,80,12)

M1=np.random.random(m1_shape)
M2=np.random.random(m2_shape)

hdf5_name_file ='python_demo.hdf5' # the hdf5_name_file

with h5py.File(hdf5_name_file, 'w') as hdf5_file: # open an hdf5 file in write mode
...      dset_name='Matrix_one'
...      dset=hdf5_file.create_dataset(dset_name, data=M2, compression='gzip')
...      dset.attrs['maxvalue']=np.max(np.max(M1))
...      dset.attrs['minvalue']=np.min(np.min(M1))
...      dset.attrs['average']=np.mean(np.mean(M1))
...      dset.attrs['var']='M1'

with h5py.File(hdf5_name_file, 'a') as hdf5_file: # open an hdf5 file in write mode
...      gzero=hdf5_file.create_group('DataTwo')
...      nset_name='Matrix_two'
...      nsdet=gzero.create_dataset(nset_name,data=M2,compression='gzip')
...      nsdet.attrs['maxvalue']=np.max(np.max(M2))
...      nsdet.attrs['minvalue']=np.min(np.min(M2))
...      nsdet.attrs['average']=np.mean(np.mean(M2))
...      nsdet.attrs['var']='M2'
``` 
