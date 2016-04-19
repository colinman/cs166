#include "kmp.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct matcher {
	int length;
	char *pattern; 
	int *suffix_ptrs;
};

// Calculates the suffix pointers
static int calc_suffix_ptr(int curr, struct matcher *my_matcher, char to_match){
	int curr_suff = my_matcher->suffix_ptrs[curr];
	if (my_matcher->pattern[curr_suff] == to_match) return curr_suff + 1;
	if (curr_suff == 0) return 0;
	return calc_suffix_ptr(curr_suff, my_matcher, to_match);
}


struct matcher* create_matcher_for(const char* pattern) {
	struct matcher *my_matcher = malloc(sizeof(struct matcher));
	/* Length  */
	my_matcher->length = strlen(pattern);

	/* Pattern  */
	my_matcher->pattern = malloc(sizeof(char) * my_matcher->length);
	memcpy(my_matcher->pattern, pattern, my_matcher->length);

	/* Suffix pointers */
	my_matcher->suffix_ptrs = malloc(sizeof(int) * (my_matcher->length + 1));
	my_matcher->suffix_ptrs[0] = 0; // base case
	my_matcher->suffix_ptrs[1] = 0; 
	for (int i = 2; i <= my_matcher->length; i++){
		my_matcher->suffix_ptrs[i] = calc_suffix_ptr(i-1, my_matcher,
													 my_matcher->pattern[i - 1]);
	}

	return my_matcher;		
}

void destroy_matcher(struct matcher* matcher) {
	free(matcher->pattern);
	free(matcher->suffix_ptrs);
	free(matcher);
}

/* Increments the training pointer by the desired amount to preserve context. 
	Follows the current suffix pointer (by updating index) */ 
static void preserve_context(const char **ptr_to_training, int *ptr_to_index, int suff_index){
	*ptr_to_training += *ptr_to_index - suff_index; // Increment training pointer to preserve context
	*ptr_to_index = suff_index;
}

void for_each_match(struct matcher* matcher,
		    const char* text,
		    void (*callback)(const char* match, void* aux),
		    void* aux) {
	int index = 0; // current index within matcher	
	const char *trailing = text; 
	const char *leading = text; 

	while (*leading != '\0'){
		/* Match */ 
		if (*leading == matcher->pattern[index]){
			/* Go to the next character */
			index ++; 
			leading ++;

			/* If a full match is found */
			if (index == matcher->length){ 
				callback(trailing, aux);
				preserve_context (&trailing, &index, matcher->suffix_ptrs[index]);
			}	
		}else{
			/* Mismatch */
			if (leading == trailing){
				/* Move both text pointers forward */
				leading ++; 
				trailing ++;
			}else{
				/* Preserve as much context as possible */
				preserve_context (&trailing, &index, matcher->suffix_ptrs[index]);	
			}	
		}
	}
}







