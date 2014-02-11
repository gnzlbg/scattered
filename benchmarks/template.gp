set title "Accessing one data member"
set xlabel "#of iterations [-]"
set ylabel "Speed-up w.r.t. std::vector [-]"

plot '< paste scattered_vector_very_small_object_multiply_by_itself_one_sequential.dat std_vector_very_small_object_multiply_by_itself_one_sequential.dat' u 1 : ($4/$2) w boxes t "very_small", '< paste scattered_vector_small_object_multiply_by_itself_one_sequential.dat std_vector_small_object_multiply_by_itself_one_sequential.dat' u 1 : ($4/$2) w boxes t "small" , '< paste scattered_vector_medium_object_multiply_by_itself_one_sequential.dat std_vector_medium_object_multiply_by_itself_one_sequential.dat' u 1 : ($4/$2) w boxes t "medium" , '< paste scattered_vector_large_object_multiply_by_itself_one_sequential.dat std_vector_large_object_multiply_by_itself_one_sequential.dat' u 1 : ($4/$2) w boxes t "large" 

set terminal svg size 400,300 enhanced font 'Tahoma,10'
set output 'res.svg'
replot
set output 'dummy.svg'
