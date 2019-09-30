/* We don't need a definition since the lseek64 function is what we need.  */
/* llseek doesn't have a prototype.  Since the second parameter is a
   64bit type, this results in wrong behaviour if no prototype is
   provided.  */
link_warning (llseek, "\
the `llseek' function may be dangerous; use `lseek64' instead.")
