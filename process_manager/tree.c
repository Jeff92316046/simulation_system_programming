
void printProcessTree(PROC *node, int depth) {
  printf("\033[0;32;34m");
  if (node == NULL) return;
  for (int i = 0; i < depth; i++) {
    printf("    ");
  }
  if (node->pid == 0)
	  printf("P%d: %s\n", node->pid,
         node == running ? "RUNNING" : pstatus[node->status]);
  else 
	  printf("└─ P%d: %s\n", node->pid,
         node == running ? "RUNNING" : pstatus[node->status]);
  printProcessTree(node->child, depth + 1);
  printProcessTree(node->sibling, depth);
  printf("\033[0m");
}