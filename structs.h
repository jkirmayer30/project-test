struct profile{
        char username[128];
        char password[128];
        int user_id;
        int match_id; // -1 is offline, -2 for waiting for match, else is userid of matched user
        //int status; // 0 is offline, 1 is online waiting to match, 2 is already matched
	int byte_total;
};

struct request{
	struct profile prof;
	char payload[512];
	int type; // 0 is registering/logging into account. 1 is sending payload. 2 would be selecting match?
};
