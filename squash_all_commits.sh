#!/bin/bash

# Check if the current directory is a Git repository
if ! git rev-parse --git-dir > /dev/null 2>&1; then
    echo "Error: Not a Git repository."
    exit 1
fi

# Get the first commit hash
FIRST_COMMIT=$(git rev-list --max-parents=0 HEAD)

# Check if we got the first commit hash
if [ -z "$FIRST_COMMIT" ]; then
    echo "Error: Cannot find the first commit."
    exit 1
fi

# Start an interactive rebase from the first commit
echo "Starting interactive rebase from the first commit..."
git rebase -i "$FIRST_COMMIT" --root

# Check if rebase was successful
if [ $? -eq 0 ]; then
    echo "Rebase completed successfully."
else
    echo "Rebase failed. Please resolve any conflicts and complete the rebase process."
fi
