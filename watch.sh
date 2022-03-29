inotifywait -e close_write,moved_to,create -m src lib |
while read -r directory events filename; do
  bazelisk build //src:*
done