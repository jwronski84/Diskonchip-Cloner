#pragma once
#include <stdint.h>
#include <stdbool.h>

typedef enum {
    WF_WAIT_FOR_MEDIA = 0,
    WF_READY_TO_FORMAT,
    WF_FORMAT_TARGET_CONFIRM,
    WF_FORMAT_TARGET_RUNNING,
    WF_READY_TO_CLONE,
    WF_BACKUP_SOURCE_TO_SD2,
    WF_CLONE_SD2_TO_TARGET,
    WF_VERIFY_TARGET,
    WF_COMPLETE,
    WF_ERROR
} workflow_state_t;

typedef struct {
    bool source_present;
    bool target_present;
    bool source_target_compatible;
    bool sd1_present;
    bool sd2_present;
    bool trueffs_available;
    bool edc_ecc_available;
    uint32_t current_block;
    uint32_t total_blocks;
    uint8_t stage_percent;
    uint8_t overall_percent;
    uint32_t failed_block;
    workflow_state_t state;
} workflow_context_t;

void workflow_init(workflow_context_t *ctx);
void workflow_tick(workflow_context_t *ctx);
void workflow_request_format(workflow_context_t *ctx);
void workflow_confirm_format(workflow_context_t *ctx);
void workflow_request_clone(workflow_context_t *ctx);
