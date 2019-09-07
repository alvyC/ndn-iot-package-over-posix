set(DIR_FACE "${DIR_NDN_LITE}/psync-fconsumer")
target_sources(ndn-lite PUBLIC
  ${DIR_FACE}/full-consumer.c
)
unset(DIR_FACE)
