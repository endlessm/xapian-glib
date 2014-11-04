// simplesearch.js - port of simplesearch.c to JS using GJS
//
// you need to run this script after running `make install` from the examples/
// directory where the testdb database lives.

const Xapian = imports.gi.Xapian;

const query_string = "a";
const QUERY_PARSER_FLAGS = Xapian.QueryParserFeature.DEFAULT | Xapian.QueryParserFeature.WILDCARD;
const path = 'examples/pagerankdb';
const POSTING_SOURCE_SLOT_NUMBER = 1;

let db = new Xapian.Database({
    'path': path
}); 
db.init(null);
let pageRank = new Xapian.ValueWeightPostingSource({slot: POSTING_SOURCE_SLOT_NUMBER});
pageRank.init(null);

let stopper = new Xapian.SimpleStopper();
stopper.add('drop');
stopper.add('me');
let qp = new Xapian.QueryParser({
    'database': db,
});
qp.stopper = stopper;
let parsed_query = qp.parse_query(query_string, QUERY_PARSER_FLAGS);
let posting_source = Xapian.Query.new_from_posting_source(pageRank);

parsed_query = Xapian.Query.new_for_pair(Xapian.QueryOp.AND_MAYBE, parsed_query, posting_source);

print('parsed_query', parsed_query.get_description());

let enquire = new Xapian.Enquire({
    'database': db
});
enquire.init(null);
enquire.set_query(parsed_query, parsed_query.get_length());

let matches = enquire.get_mset(0, 10);
print('matches_size', matches.get_size())

let iter = matches.get_begin();
while (iter.next()) {
    let result = '' + iter.get_rank() + 1 + ': '
               + iter.get_weight() + ' '
               + 'docid=' + iter.get_doc_id() + ' '
               + 'title=' + JSON.parse(iter.get_document().get_data()).title;

    print('result', result);
}
